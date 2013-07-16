# -*- coding: utf-8 -*-

from id_service import alloc_id

def parser(line_tokens):
    COL_TYPENAME = 0
    COL_ANINAME = 1
    COL_STATUS = 2
    COL_FRAME_INDEX = 3
    COL_FRAME_ELAPSED = 4
    COL_FRAME_COMPONENT = 5

    data = type("Data", (object,), dict
            ( typename = None
            , aniname = None
            , status = None
            , actions = None
            , action_name = None
            , frames = None
            , expect_frame_index = 1
            , frame_index = None
            , frame_elapsed = None
            , components = None
            , results = []
            ))

    def resolve_animation(data):
        if data.typename:
            data.results.append({
                'typename': data.typename,
                'actions': data.actions,
                })
        data.typename = None
        data.actions = []

    def resolve_action(data):
        if data.frames and data.status != u'禁用':
            data.actions.append({
                'action_name': data.action_name,
                'frames': data.frames,
                })

        data.action_name = None
        data.status = None
        data.frames = []
        data.expect_frame_index = 1

    def resolve_frame(data):
        if data.components:
            assert(data.frame_index == data.expect_frame_index)
            data.expect_frame_index += 1
            data.frames.append({
                'elapsed': int(data.frame_elapsed),
                'components': data.components,
                })

        data.frame_elapsed = None
        data.components = []

    line_tokens = line_tokens[2:] # skip header
    for row, tokens in enumerate(line_tokens):
        if tokens[COL_FRAME_INDEX]:
            resolve_frame(data)

            data.frame_elapsed = tokens[COL_FRAME_ELAPSED]
            data.frame_index = int(tokens[COL_FRAME_INDEX])

        if tokens[COL_ANINAME]:
            resolve_action(data)
            data.action_name = tokens[COL_ANINAME]
            data.status = tokens[COL_STATUS]

        if tokens[COL_TYPENAME]:
            resolve_animation(data)
            data.typename = tokens[COL_TYPENAME]

        data.components.append(int(tokens[COL_FRAME_COMPONENT])-1)
    resolve_frame(data)
    resolve_action(data)
    resolve_animation(data)

    return data.results

def animation_handler(sheet_name, line_tokens):
    results = parser(line_tokens)
    # from pprint import pprint
    # pprint(results)
    return results

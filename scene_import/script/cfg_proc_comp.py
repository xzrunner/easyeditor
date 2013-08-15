# -*- coding: utf-8 -*-

from animation_proto import AnimationProto

def parser(line_tokens):
    COL_TYPENAME = 0
    COL_STATUS = 1
    COL_INDEX = 3
    COL_TYPE = 4
    COL_NAME = 5
    COL_MIRROR = 7

    data = type("Data", (object,), dict
            ( typename = None
            , status = None
            , components = None
            , results = []
            ))

    def resolve(data):
        if data.typename and data.status != u'禁用':
            ap = AnimationProto()
            if data.status == u'导出':
                ap.export = data.typename
            ap.typename = data.typename
            ap.components = data.components
            data.results.append(ap)
            # id_animation_map[ap.id] = ap
            # name_part_map[ap.typename] = ap
        data.typename = None
        data.status = None
        data.components = []

    line_tokens = line_tokens[2:] # skip header
    for row, tokens in enumerate(line_tokens):
        if tokens[COL_TYPENAME]:
            resolve(data)
            data.typename = tokens[COL_TYPENAME]
            data.status = tokens[COL_STATUS]

        if tokens[COL_NAME]:
            data.components.append({
                'index': int(tokens[COL_INDEX]) - 1,
                'type': tokens[COL_TYPE],
                'name': tokens[COL_NAME],
                'mirror': tokens[COL_MIRROR] == 1,
                'pic': None,
                })
    resolve(data)

    return data.results

def component_handler(sheet_name, line_tokens, all_animations):
    results = parser(line_tokens)
    # from pprint import pprint
    # pprint(results)
    all_animations.extend(results)


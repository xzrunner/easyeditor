# -*- coding: utf-8 -*-

from id_service import alloc_id
from animation_proto import AnimationProto

# def parser1(line_tokens):
#     COL_ID = 0
#     COL_NAME = 1
#     COL_DISABLE = 2
#     COL_LEVEL = 3
#     COL_COMP_INDEX = 4
#     COL_COMP_TYPE = 5
#     COL_COMP_NAME = 6
# 
#     data = type("Data", (object,), dict
#             ( id = None
#             , disable = None
#             , level = None
#             , components = []
#             , results = []
#             ))
# 
#     def resolve(data):
#         if data.level and data.disable != u'是':
#             ap = AnimationProto()
#             ap.id = alloc_id()
#             ap.export = "building_%d_lv%d" % (data.id, data.level)
#             ap.components = data.components
#             data.results.append(ap)
#             id_animation_map[ap.id] = ap
#         data.level = None
#         data.components = []
# 
#     line_tokens = line_tokens[2:] # skip header
#     for row, tokens in enumerate(line_tokens):
#         if tokens[COL_LEVEL]:
#             resolve(data)
#             data.level = tokens[COL_LEVEL]
# 
#         if tokens[COL_ID]:
#             data.id = tokens[COL_ID]
#             data.disable = tokens[COL_DISABLE]
# 
#         if tokens[COL_COMP_NAME]:
#             data.components.append({
#                 'index': int(tokens[COL_COMP_INDEX]) - 1,
#                 'type':  tokens[COL_COMP_TYPE],
#                 'name':  tokens[COL_COMP_NAME],
#                 })
# 
#     return data.results
# 
# def handler1(sheet_name, line_tokens, all_animations):
#     results = parser1(line_tokens)
#     from pprint import pprint
#     pprint(results)
#     all_animations.extend(results)

def parser(line_tokens):
    COL_TYPENAME = 0
    COL_STATUS = 1
    COL_INDEX = 3
    COL_TYPE = 4
    COL_NAME = 5

    data = type("Data", (object,), dict
            ( typename = None
            , status = None
            , components = None
            , results = []
            ))

    def resolve(data):
        if data.typename and data.status != u'禁用':
            ap = AnimationProto()
            ap.id = alloc_id()
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
                })
    resolve(data)

    return data.results

def component_handler(sheet_name, line_tokens, all_animations):
    results = parser(line_tokens)
    # from pprint import pprint
    # pprint(results)
    all_animations.extend(results)


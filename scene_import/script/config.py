# -*- coding: utf-8 -*-

class Config(dict):
    def __setitem__(self, k, v):
        self[k] = v

    def __getitem__(self, k):
        return self[k]

config = Config()

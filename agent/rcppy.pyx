# distutils: language = c++

from rcp cimport rcp
from libcpp cimport bool
ctypedef unsigned char char_type

cdef class rcppy:
    cdef rcp c_rcp

    def __cinit__(self):
        self.c_rcp = rcp()

    def connect(self, ip, int port, bool block = False):
        self.c_rcp.connect(ip.encode(),port,block)

    def serve(self,int port):
        self.c_rcp.serve(port)

    def listen(self,bool conn = False, bool block = False):
        self.c_rcp.listen(conn, block)


# distutils: language = c++

from rcp cimport rcp
ctypedef unsigned char char_type

cdef class PyRCP:
    cdef rcp c_rcp

    def __cinit__(self):
        self.c_rcp = rcp()

    def connect(self, ip, int port, int block):
        self.c_rcp.connect(ip.encode(),port,block)

    def listen(self,int port, int block):
        self.c_rcp.rcp_listen(port,block)

    def init(self):
        self.c_rcp.init()

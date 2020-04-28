# distutils: language = c++

from rcp cimport rcp
from libcpp cimport bool
ctypedef unsigned char char_type

cdef class rcppy:
    cdef rcp c_rcp

    def __cinit__(self):
        self.c_rcp = rcp()

    def connect(self, ip, port, block = False):
        self.c_rcp.connect(ip.encode(),port,block)

    def serve(self, port):
        self.c_rcp.serve(port)

    def listen(self,buffsize, conn = False, block = False):
        self.c_rcp.listen(buffsize,conn, block)

    def send(self,msg,ip, port):
        self.c_rcp.send(msg, ip.encode(), port)
    
    def read(self):
        return self.c_rcp.read()
        


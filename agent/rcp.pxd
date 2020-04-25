cdef extern from "rcp.cpp":
    pass

cdef extern from "rcp.h":
    cdef cppclass rcp:
        rcp() except +
        void connect(const char* ip, int port, int block)
        void rcp_listen(int port, int block)
        void init()
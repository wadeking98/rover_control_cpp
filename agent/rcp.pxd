from libcpp cimport bool
cdef extern from "rcp.cpp":
    pass

cdef extern from "rcp.h":
    cdef cppclass rcp:
        rcp() except +
        void connect(const char* ip, int port, bool block)
        void rcp_listen(int port, bool block)

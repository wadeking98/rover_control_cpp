from libcpp cimport bool
cdef extern from "rcp.cpp":
    pass

cdef extern from "rcp.h":
    cdef cppclass rcp:
        rcp() except +
        void connect(const char* ip, int port, bool block)
        void serve(int port)
        void listen(int buffsize, bool conn, bool block)
        void send(const char* msg, const char* ip, int port)

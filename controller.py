import rcppy
import sys

if len(sys.argv) <2:
    print("Usage: python3 controller.py <rover_ip>")
    exit(1)

agent = rcppy.PyRCP()
agent.connect(sys.argv[1],5555,False)
while True:
    pass
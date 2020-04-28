import rcppy
import time
agent = rcppy.rcppy()
agent.serve(5555)
agent.listen(1024,True,False)
while True:
    time.sleep(1)
    print(agent.read())
    pass
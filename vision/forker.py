from multiprocessing import Process, Pipe

from server import *


if __name__ == '__main__':
    parent_conn, child_conn = Pipe()
    p = Process(target=run, args=(child_conn, ))
    print 'Running server'
    p.start()
    print 'Sending signal'
    parent_conn.send('Fuck yeah!')
    p.join()

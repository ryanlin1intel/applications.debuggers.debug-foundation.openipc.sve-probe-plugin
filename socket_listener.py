from multiprocessing.connection import Listener



if __name__ == "__main__":
    address = ('localhost', 8080)

    listener = Listener(address, authkey=b'sve')

    conn = listener.accept()
    print("Connection accepted from: ", listener.last_accepted)

    while True:
        msg = conn.recv()
        print(f"MSG: {msg}")
        if msg == 'close':
            print("Closing connection")
            conn.send("Closing Connection")
            conn.close()
            break
        else:
            conn.send(0xdeadbeef)

    listener.close()
            

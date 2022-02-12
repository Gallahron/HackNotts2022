package Server;

import Main.Main;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;

public class Server {
    int portNo = 6969;
    DatagramSocket aSocket;

    public Server(int portNo) {
        this.portNo = portNo;

        try {
            System.out.println(InetAddress.getLocalHost());
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }

        try {
            aSocket = new DatagramSocket(portNo);
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

    public void OpenServer() {
        try{
            byte[] buffer = new byte[1000];
            while(true){
                DatagramPacket request = new DatagramPacket(buffer, buffer.length);
                aSocket.receive(request);

                String connectionString = new String(request.getData(), StandardCharsets.UTF_8);
                String connectionType = connectionString.substring(0,4);
                System.out.println("Request Recieved: " + connectionType);

                switch (connectionType) {
                    case ("CONN"):
                        ProcessConnection(connectionString, request);
                        break;
                    case ("INPT"):
                        ProcessInput(connectionString, request);
                        break;
                    default:
                        ConnectionError(request);
                        break;
                }
            }
        } catch (IOException e) {System.out.println("IO: " + e.getMessage());
        } finally {if(aSocket != null) aSocket.close();}
    }

    public void SendMessage(String message, DatagramPacket request) {
        SendMessage(message, request.getAddress(), request.getPort());
    }

    public void SendMessage(String message, InetAddress address, int port) {
        try {
            DatagramPacket reply = new DatagramPacket(message.getBytes(),
                    message.length(), address, port);
            aSocket.send(reply);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    void ConnectionError(DatagramPacket request) {
        SendMessage("Please ensure request is in the correct format", request);
    }

    void ProcessConnection(String data, DatagramPacket request) {
        StringBuilder builder = new StringBuilder();

        for (char i : data.toCharArray()) {
            switch (i) {
                case (']'):
                    int playerNo = 0;
                    try {
                        playerNo = PlayerManager.AddPlayer(Integer.parseInt(builder.toString()), request.getAddress(), request.getPort());
                    } catch (NumberFormatException e) {
                        ConnectionError(request);
                        return;
                    }
                    System.out.println("MachineID: " + builder);

                    InputStates.AddState(playerNo);
                    SendMessage("ACCP[PN" + playerNo + "]", request);
                    return;
                case ('['):
                    builder = new StringBuilder();
                    break;
                default:
                    builder.append(i);
            }

            if (builder.toString().equals("ID")) {
                builder = new StringBuilder();
            }
        }
    }

    void ProcessInput(String data, DatagramPacket request) {
        StringBuilder builder = new StringBuilder();
        int playerNo = 0;
        String element = "";

        InputState state = new InputState();

        loop: for (char i : data.toCharArray()) {
            int sym = (int) i;
            if (((sym > 47 && sym < 58) || sym == 45) && element.isEmpty()) {
                element = builder.toString();
                builder = new StringBuilder();
                builder.append(i);
            } else {
                switch (i) {
                    case (']'):
                    case ('_'):
                        int value = Integer.parseInt(builder.toString());
                        if (element.equals("ID")) {
                            playerNo = PlayerManager.playerLookup.get(value).getPlayerNumber();
                            value = playerNo;
                            try {
                                InputStates.available.acquire();
                                state = InputStates.GetState(playerNo);
                                InputStates.available.release();
                            } catch (InterruptedException e){}
                        }
                        state.UpdateState(element, value);
                        builder = new StringBuilder();
                        element = "";

                        if (i == ']') break loop;
                        break;
                    case ('['):
                        builder = new StringBuilder();
                        break;
                    default:
                        builder.append(i);
                }
            }
        }
        try {
            InputStates.available.acquire();
            InputStates.AddState(playerNo, state);
            System.out.print(InputStates.GetState(playerNo).ToString());
            InputStates.available.release();
        } catch (InterruptedException e){};
    }
}

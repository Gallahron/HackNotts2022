package Main;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.concurrent.Callable;

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
                    default:
                        ConnectionError(request);
                        break;
                }
            }
        } catch (IOException e) {System.out.println("IO: " + e.getMessage());
        } finally {if(aSocket != null) aSocket.close();}
    }

    void SendMessage(String message, DatagramPacket request) {
        try {
            DatagramPacket reply = new DatagramPacket(message.getBytes(),
                    message.length(), request.getAddress(), request.getPort());
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
                case ('/'):
                    int playerNo = Main.AddPlayer(Integer.parseInt(builder.toString()));
                    System.out.println("MachineID: " + builder);

                    InputStates.AddState(playerNo);
                    SendMessage("ACCP-PN" + playerNo + "/-END", request);
                    break;
                case ('-'):
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
        for (char i : data.toCharArray()) {
            if ((int)i > 47 && (int)i < 58 && element.isEmpty()) {
                element = builder.toString();
                builder = new StringBuilder();
                builder.append(i);
            } else {
                switch (i) {
                    case ('/'):
                        int value = Integer.parseInt(builder.toString());
                        if (element.equals("ID")) {
                            playerNo = Main.playerLookup.get(value);
                            value = playerNo;
                        }
                        InputStates.ModifyState(playerNo, element, value);
                        builder = new StringBuilder();
                        element = "";
                        break;
                    case ('-'):
                        builder = new StringBuilder();
                        break;
                    default:
                        builder.append(i);
                }
            }
        }
        System.out.print(InputStates.GetState(playerNo).ToString());
    }
}

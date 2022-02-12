package Main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;

public class GetIP {


    public static ArrayList<String> getLocalHostAddresses() {

        ArrayList<String> addresses = new ArrayList<String>();

        try {
            Enumeration<NetworkInterface> e = NetworkInterface.getNetworkInterfaces();

            while (e.hasMoreElements()) {
                NetworkInterface ni = e.nextElement();
                Enumeration<InetAddress> e2 = ni.getInetAddresses();
                while (e2.hasMoreElements())
                    addresses.add(e2.nextElement().getHostAddress());
            }
            URL whatismyip = new URL("http://checkip.amazonaws.com");

            BufferedReader in = null;
            in = new BufferedReader(new InputStreamReader(
                    whatismyip.openStream()));

            addresses.add(in.readLine());
        } catch (Exception ignore) {
        }

        return addresses;
    }
}

package Server;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class InputState {
    private HashMap<String, Integer> state = new HashMap<String, Integer>();
    public InputState() {
        state.put("ID", 0);
        state.put("L", 0);
        state.put("R", 0);
        state.put("J", 0);
        state.put("S", 0);
        state.put("E", 0);
    }

    public boolean shootToggle;
    public boolean jumpToggle;

    public void UpdateState(String key, Integer value) {
        if (state.containsKey(key)) {
            state.put(key, value);
        } else {
            System.out.println("Key does not exist");
        }
    }

    public int GetState(String key) {
        if (state.containsKey(key)) {
            return state.get(key);
        } else {
            System.out.println("Key does not exist");
            return -1;
        }
    }

    public String ToString() {
        String output = "";
        for (Map.Entry<String, Integer> entry : state.entrySet()) {
            output += entry.getKey() + ": " + entry.getValue().toString() + ", ";
        }
        return output;
    }

}

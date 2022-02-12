package Main;

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

    public void UpdateState(String key, Integer value) {
        if (state.containsKey(key)) {
            state.put(key, value);
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

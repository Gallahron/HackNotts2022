package Main;

import java.util.ArrayList;
import java.util.HashMap;

public class InputStates {
    private static HashMap<Integer, InputState> inputs = new HashMap<Integer, InputState>();

    public static void AddState(int playerNo, InputState state) {
        inputs.put(playerNo, state);
    }
    public static void AddState(int playerNo) {
        AddState(playerNo, new InputState());
    }

    public static void ModifyState(int playerNo, String key, int value) {
        inputs.get(playerNo).UpdateState(key, value);
    }

    public static InputState GetState(int playerNo) {
        return inputs.get(playerNo);
    }

}

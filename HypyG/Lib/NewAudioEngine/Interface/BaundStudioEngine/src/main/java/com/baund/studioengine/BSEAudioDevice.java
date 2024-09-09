/*
 * Baund Studio Engine
 *
 * BSEAudioDevice.java
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

package com.baund.studioengine;

import android.content.Context;
import android.media.AudioDeviceInfo;
import android.media.AudioManager;

import java.util.ArrayList;

public class BSEAudioDevice {
    static {
        System.loadLibrary("studioengine");
    }
    // 오디오기기 사용을 위한 초기화를 진행합니다.
    public static native void ready();
    // 오디오기기의 작동을 시작합니다.
    public static native void start();
    // 오디오기기의 작동을 정지합니다.
    public static native void stop();
    // 오디오기기의 설정을 해제합니다.
    public static native void close();
    // 오디오기기의 초기화여부를 반환합니다.
    public static native boolean isReady();
    // 오디오기기가 작동중인지를 반환합니다.
    public static native boolean isStart();
    // 오디오기기의 지연시간을 반환합니다.
    public static native double getLatency();

    public static String reciever     = "reciever";
    public static String speaker      = "speaker";
    public static String bluetooth    = "bluetooth";
    public static String headphones   = "headphones";
    public static String builtinMic   = "builtinMic";
    public static String bluetoothMic = "bluetoothMic";
    public static String headsetMic   = "headsetMic";

    private static Context context;

    public static void setContext(Context in) {
        context = in;
    }
    public static class DeviceIO {
        public ArrayList<String> inputs;
        public ArrayList<String> outputs;
    };
    public static DeviceIO getIOList() {
        DeviceIO deviceIO = new DeviceIO();
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        AudioDeviceInfo[] inputs  = audioManager.getDevices(AudioManager.GET_DEVICES_INPUTS);
        AudioDeviceInfo[] outputs = audioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS);
        for (int i=0; i<inputs.length; i++) {
            if (inputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_MIC) {
                deviceIO.inputs.add(builtinMic + inputs[i].getAddress());
            } else if (inputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADSET ||
                       inputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADPHONES) {
                deviceIO.inputs.add(headsetMic);
            } else if (inputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_SCO ||
                       inputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_A2DP) {
                deviceIO.inputs.add(bluetoothMic);
            }
        }
        for (int i=0; i<outputs.length; i++) {
            if (outputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_EARPIECE) {
                deviceIO.outputs.add(reciever);
            } else if (outputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_SPEAKER) {
                deviceIO.outputs.add(speaker);
            } else if (outputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADSET ||
                       outputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADPHONES) {
                deviceIO.outputs.add(headphones);
            } else if (outputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_SCO ||
                       outputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_A2DP) {
                deviceIO.outputs.add(bluetooth);
            }
        }
        return deviceIO;
    }
    public static boolean use(String input, String output) {
        DeviceIO deviceIO = new DeviceIO();
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        AudioDeviceInfo[] inputs  = audioManager.getDevices(AudioManager.GET_DEVICES_INPUTS);
        AudioDeviceInfo[] outputs = audioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS);
        int inputID  = -1;
        int outputID = -1;
        for (int i=0; i<inputs.length; i++) {
            if (input == builtinMic + inputs[i].getAddress()) {
                if (inputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_MIC) {
                    inputID = inputs[i].getId();
                    break;
                }
            } else if (input == bluetoothMic) {
                if (inputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_SCO ||
                    inputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_A2DP) {
                    inputID = inputs[i].getId();
                    break;
                }
            } else if (input == headsetMic)  {
                if (inputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADSET ||
                    inputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADPHONES) {
                    inputID = inputs[i].getId();
                    break;
                }
            }
        }
        for (int i=0; i<outputs.length; i++) {
            if (output == reciever) {
                if (outputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_EARPIECE) {
                    outputID = outputs[i].getId();
                }
            } else if (output == speaker) {
                if (outputs[i].getType() == AudioDeviceInfo.TYPE_BUILTIN_SPEAKER) {
                    outputID = outputs[i].getId();
                }
            } else if (output == bluetooth) {
                if (outputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_SCO ||
                    outputs[i].getType() == AudioDeviceInfo.TYPE_BLUETOOTH_A2DP) {
                    outputID = outputs[i].getId();
                    break;
                }
            } else if (output == headphones) {
                if (outputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADSET ||
                    outputs[i].getType() == AudioDeviceInfo.TYPE_WIRED_HEADPHONES) {
                    outputID = outputs[i].getId();
                    break;
                }
            }
        }
        if (inputID > -1 && outputID > -1) {
            setID(inputID,outputID);
            return true;
        } else {
            return false;
        }
    }
    private static native void setID(int inputID, int outputID);
}

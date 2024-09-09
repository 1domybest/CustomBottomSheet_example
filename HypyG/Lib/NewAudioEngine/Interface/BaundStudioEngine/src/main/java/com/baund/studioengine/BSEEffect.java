package com.baund.studioengine;

public class BSEEffect {
    public static String none = "";
    public static class Tone {
        public static String bright = "tone-bright";
        public static String stuffy = "tone-stuffy";
        public static String thin   = "tone-thin";
        public static String light  = "tone-light";
    }
    public static class Distortion {
        public static String fourbit = "distortion-4bit";
        public static String lowbit  = "distortion-lowbit";
    }
    public static class Delay {
        public static String halfNote         = "delay-1/2";
        public static String quarterNote      = "delay-1/4";
        public static String dottedEighthNote = "delay-1/6";
        public static String eighthNote       = "delay-1/8";
    }
    public static class Reverb {
        public static String shorter = "reverb-shorter";
        public static String medium  = "reverb-medium";
        public static String longer  = "reverb-longer";
    }
}

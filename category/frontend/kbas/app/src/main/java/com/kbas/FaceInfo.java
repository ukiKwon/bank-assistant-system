package com.kbas;

class FaceInfo extends BaseModel{
    private final int EMO_NUM = 8;
    private float[] emotionsChange = new float[EMO_NUM];//emotions
    private int mouthOpenCount;//how many tallk
    private int eyeBlicnkCount;//how many eyes will blink
    private int HeadLocCount;//a change of a location of head

    public FaceInfo(){};
    public void setEmotionsChange(float[] emotionsChange) {
        this.emotionsChange = emotionsChange;
    }

    public void setHeadLocChange(int headLocChange) {
        HeadLocCount = headLocChange;
    }

    public void setEyeBlicnkCount(int eyeBlicnkCount) {
        this.eyeBlicnkCount = eyeBlicnkCount;
    }

    public void setMouthChange(int mouthOpenCount) {
        this.mouthOpenCount = mouthOpenCount;
    }

    public float getEyeBlicnkChange() {
        return eyeBlicnkCount;
    }

    public float getHeadLocChange() {
        return HeadLocCount;
    }

    public float getMouthChange() {
        return mouthOpenCount;
    }

    public float[] getEmotionsChange() {
        return emotionsChange;
    }
    public void updateEmotions(float[] newEmotions) {
        for (int i = 0; i < EMO_NUM; ++i)
            emotionsChange[i] = (emotionsChange[i] + newEmotions[i])/2;
    }
}

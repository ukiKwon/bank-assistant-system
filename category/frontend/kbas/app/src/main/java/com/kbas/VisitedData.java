package com.kbas;

public class VisitedData extends BaseModel {
    private String custom_id;//고객 식별자
    private String custom_name;//고객 이름
    private String curFinService;//현재 상담받고 있는 상담 종류
    private long onServiceTime;//현재 상담 진행 시간
    private long onWaitTime;//상담 전 기다린 시간
    private FaceInfo faceInfo;//얼굴 정보
    private VoiceInfo voiceInfo;//음성 정보

    public VisitedData() {;}
    public VisitedData(String custom_id, String custom_name) {
        this.custom_id = custom_id;
        this.custom_name = custom_name;
    }

    public void setFaceInfo(FaceInfo faceInfo) {
        this.faceInfo = faceInfo;
    }
    public void setVoiceInfo(VoiceInfo voiceInfo) {
        this.voiceInfo = voiceInfo;
    }
    public void setCurFinService(String curFinService) {
        this.curFinService = curFinService;
    }
    public void setCustomId(String custom_id) {
        this.custom_id = custom_id;
    }
    public void setCustomName(String custom_name) {
        this.custom_name = custom_name;
    }
    public void setOnServiceTime(long onServiceTime) {
        this.onServiceTime = onServiceTime;
    }
    public void setOnWaitTime(long onWaitTime) {
        this.onWaitTime = onWaitTime;
    }

    public String getCurFinService() {
        return curFinService;
    }
    public String getCustomId() {
        return custom_id;
    }
    public String getCustomName() {
        return custom_name;
    }
    public FaceInfo getFaceInfo() {
        return faceInfo;
    }
    public VoiceInfo getVoiceInfo() {
        return voiceInfo;
    }
    public long getOnWaitTime() {
        return onWaitTime;
    }
    public long getOnServiceTime() {
        return onServiceTime;
    }
}

package com.kbas.Model;

public class VisitedData extends BaseModel {
    private String custom_id;//고객 식별자
    private String custom_name;//고객 이름
    private String curFinService;//현재 상담받고 있는 상담 종류
    private long onServiceTime = 0;//현재 상담 진행 시간
    private long onWaitTime = 0;//상담 전 기다린 시간
    private FaceInfo faceAccumInfo = new FaceInfo();//얼굴 정보
    private VoiceInfo voiceAccumInfo = new VoiceInfo();//음성 정보

    public VisitedData() {;}
    public VisitedData(String custom_id, String custom_name) {
        this.custom_id = custom_id;
        this.custom_name = custom_name;
    }
    public void setFaceInfo(FaceInfo faceInfo) {
        this.faceAccumInfo = faceInfo;
    }
    public void setVoiceInfo(VoiceInfo voiceInfo) {
        this.voiceAccumInfo = voiceInfo;
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
    public void addVoice(VoiceUnit voiceunit) {
        voiceAccumInfo.addVoiceUnit(voiceunit);
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
        return faceAccumInfo;
    }
    public VoiceInfo getVoiceInfo() {
        return voiceAccumInfo;
    }
    public long getOnWaitTime() {
        return onWaitTime;
    }
    public long getOnServiceTime() {
        return onServiceTime;
    }
    public void describes() {
        System.out.println("custom_id" + custom_id);
        System.out.println("custom_name" + custom_name);
        System.out.println("curFinService" + curFinService);
        System.out.println("onServiceTime" + onServiceTime);
        System.out.println("onWaitTime" + onWaitTime);
        System.out.println("custom_id" + custom_id);
        if (faceAccumInfo != null) faceAccumInfo.describes();
        if (voiceAccumInfo != null) faceAccumInfo.describes();
    }
}

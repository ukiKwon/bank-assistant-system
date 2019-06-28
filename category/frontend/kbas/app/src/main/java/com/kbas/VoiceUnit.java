package com.kbas;

import java.text.SimpleDateFormat;
import java.util.Date;

public class VoiceUnit {
    private VoiceInfo.SPEAKER speaker;
    private final int MAX_SENTENCE = 10;
    private Date starTime, endTime;
    private SimpleDateFormat mFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
    private String[] sentences = new String[MAX_SENTENCE];

    public VoiceUnit(){;}
    public VoiceUnit(VoiceInfo.SPEAKER speaker) {this.speaker = speaker;}
    public VoiceUnit(long startTime) {
        this.starTime = new Date(startTime);
    }
    public void setSpeaker(VoiceInfo.SPEAKER speaker) {
        this.speaker = speaker;
    }
    public int getSpeaker() { return this.speaker.getId();}
    public void setSentences(String[] sentences) { this.sentences = sentences; }
    public String[] getSentences() { return this.sentences;}
    public void setStartTime(long startTime) { this.starTime = new Date(startTime);}
    public String getStartTime() { return mFormat.format(starTime);}
    public void setEndTime(long endTime) { this.starTime = new Date(endTime);}
    public String getEndTime() { return mFormat.format(endTime);}
    public String getDueTime() { return mFormat.format(endTime.getTime() - starTime.getTime());}
}

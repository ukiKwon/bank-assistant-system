package com.kbas.Model;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

class VoiceInfo extends BaseModel{
    public enum SPEAKER {
        CUSTOM(0), BANKER(1), OUTSIDE(2);
        int sid;
        SPEAKER(int sid) {
            this.sid = sid;
        }
        public int getId() {
            return this.sid;
        }
    }
    private List <VoiceUnit> voiceUnits = new ArrayList<>();
    public VoiceInfo(){;}
    public VoiceInfo(VoiceUnit voiceUnit) { voiceUnits.add(voiceUnit);}
    public void addVoicUnits(List <VoiceUnit> voiceUnits) { this.voiceUnits.addAll(voiceUnits); }
    public List <VoiceUnit> getVoiceUnits() { return this.voiceUnits;}
    //todo : SPEARKER 별로 객체 구성? 고려 해볼것. 일단은 find로 찾아낼 거임.
    public List <VoiceUnit> getVoiceUnitsBySpeaker(VoiceInfo.SPEAKER speaker) {
        List <VoiceUnit> theVoice = new ArrayList<>();
        Iterator<VoiceUnit> iterator = voiceUnits.iterator();
        while (iterator.hasNext()) {
            VoiceUnit vu = iterator.next();
            //find the voice
            if (speaker.getId() == vu.getSpeaker()) {
                theVoice.add(vu);
            }
        }
        return theVoice;
    }
    public void describes() {
        System.out.println("\n######### VOICE_INFO #########");
        for (int i = 0; i < voiceUnits.size(); ++i)
            voiceUnits.get(i).describes();
    }
}

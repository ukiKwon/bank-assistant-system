package com.kbas.data.model;

import android.media.Image;
import android.os.Parcel;
import android.os.Parcelable;

import java.io.Serializable;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class Custom extends BaseModel {
    private Image IDcard;
    private String name;
    private Date lastVisitedDate;
    private String lastDoneService;
    public Custom() {};
    public void setIDcard(Image IDcard) {this.IDcard = IDcard;}
    public Image getIDcard() {return this.IDcard;}
    public void setName(String name) {this.name = name;}
    public String getName() {return this.name;}
    public void setLastVisitedDate(Date lastVisitedDate) {this.lastVisitedDate = lastVisitedDate;}
    public Date getLastVisitedDate() {return this.lastVisitedDate;}
    public void setLastDoneService(String lastDoneService) {this.lastDoneService = lastDoneService;}
    public String getLastDoneService() {return this.lastDoneService;}

    public Map< String, Object > toMap() {
        Map<String, Object> result = new HashMap<>();
        result.put("IDcard", this.getIDcard());
        result.put("name", this.getName());
        result.put("lastVisitDate", this.getLastVisitedDate());
        result.put("lastDoneService", this.getLastDoneService());
        return result;
    }
}

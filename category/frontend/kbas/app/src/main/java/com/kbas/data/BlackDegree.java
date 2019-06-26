package com.kbas.data.model;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.Image;

import com.kbas.R;

import java.io.Serializable;

public class BlackDegree extends BaseModel {
    public enum STATE {
        WHITE(R.drawable.white),
        WHITE2GRAY(R.drawable.white2gray),
        GRAY(R.drawable.gray),
        GRAY2BLACK(R.drawable.gray2black),
        BLACK(R.drawable.black);
        int state;
        STATE(int stateImage) {
            this.state = state;
        }
        public int getStateImage() {
            return this.state;
        }
    }
    public static final double mThreshold = 0.6;
    private double blackdegree = 0;
    private Bitmap blackImage;
    public BlackDegree() {;}

    public void setBlackDegree(double blackdegree) {this.blackdegree = blackdegree;}
    public double getBlackdegree() {return this.blackdegree;}
    public void setBlackImage(Bitmap blackImage) {this.blackImage = blackImage;}
    public Bitmap getBlackImage() { return this.blackImage;}
    public boolean IsBlack() { return blackdegree > mThreshold;}
}

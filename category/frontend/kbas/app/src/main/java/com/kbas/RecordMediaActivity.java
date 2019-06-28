package com.kbas;

import android.content.Intent;
import android.os.Bundle;

import com.google.gson.Gson;

import androidx.appcompat.app.AppCompatActivity;

public class RecordMediaActivity extends AppCompatActivity {
    private VisitedData mVisitedData = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        setConfig();
    }
    public void setConfig() {
        //Intent from Activity
        Intent intent = getIntent();
        String mCid = intent.getExtras().getString("cid");
        String mCname = intent.getExtras().getString("cname");
        //check data
        try {
            if (mCid != null && mCname != null) {
                mVisitedData = new VisitedData(mCid, mCname);
                mVisitedData.describes();
            } else {
                throw new Exception();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

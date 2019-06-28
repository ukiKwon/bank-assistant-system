package com.kbas;

import android.content.Intent;
import android.os.Bundle;

import com.google.gson.Gson;

import androidx.appcompat.app.AppCompatActivity;

public class RecordMediaActivity extends AppCompatActivity {
    private VisitedData mVisitedData;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        setConfig();
    }
    public void setConfig() {
        //Intent from Activity
        Intent intent = getIntent();
        String vdata = intent.getExtras().getString("vdata");
        System.out.println(">> Intent data : " + vdata);
        //check data
        try {
            if (vdata != null) {
                Gson gson = new Gson();
                mVisitedData = gson.fromJson(vdata, VisitedData.class);
                mVisitedData.describes();
            } else {
                throw new Exception();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

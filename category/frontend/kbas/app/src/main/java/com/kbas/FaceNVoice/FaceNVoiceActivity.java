package com.kbas;

import android.app.Activity;
import android.os.Bundle;

public class FaceNVoiceActivity extends Activity implements FaceNVoiceContract.View {
    FaceNVoiceContract.Presenter mPresenter;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_facenvoice);
    }
}

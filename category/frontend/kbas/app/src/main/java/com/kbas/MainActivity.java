package com.kbas;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.kbas.Camera.CameraActivity;

public class MainActivity extends AppCompatActivity {
    private Button btn_face;
    private EditText mEditText;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setConfig();
    }
    public void setConfig() {
        mEditText = (EditText) findViewById(R.id.main_edit);
        btn_face = (Button)findViewById(R.id.button_face);
        btn_face.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent=new Intent(MainActivity.this, FaceNVoiceActivity.class);
                intent.putExtra("text",String.valueOf(mEditText.getText()));
                startActivity(intent);
            }
        });
    }
}

package com.kbas;
/*
    * Camera2APIs: https://metalkin.tistory.com/92
    * bluetooth :
    * audio : https://one-delay.tistory.com/44
*/
import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.media.Image;
import android.os.Bundle;
import android.os.Handler;
import android.util.Size;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;

import com.google.firebase.ml.vision.FirebaseVision;
import com.google.firebase.ml.vision.common.FirebaseVisionImage;
import com.google.firebase.ml.vision.face.FirebaseVisionFaceDetector;
import com.google.firebase.ml.vision.face.FirebaseVisionFaceDetectorOptions;

import java.io.IOException;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

public class RecordMediaActivity extends AppCompatActivity
        implements Camera2APIs.Camera2Interface, TextureView.SurfaceTextureListener {
    ///activity
    private final long WAIT_PROCESSING_RES = 3000;
    private final int MY_PERMISSIONS_REQUEST_VOICE = 00002;
    private TextView mTextView;
    private ImageButton mImageButton;
    private int tabCount = 0;
    private long baseTime = 0;
    //to-saved
    private VisitedData mVisitedData = null;
    //camera
    private TextureView mTextureView;
    private Camera2APIs mCamera;
    private FirebaseVisionFaceDetector detector;
    //FaceRecognition
    FirebaseVisionFaceDetectorOptions highAccuracyOpts, realTimeOpts;
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
        //view-variables
        mTextView = (TextView) findViewById(R.id.textView);
        mImageButton = (ImageButton) findViewById(R.id.btn_done);
        setTextInTime(getString(R.string.sales_on), 1000); //text-switching
        mImageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Voice-function
                voicePermissionChecker();
                //Todo : 음성 인식 기능 결합
            }
        });
        //camera-view set
        mTextureView = (TextureView) findViewById(R.id.textureView);
        mTextureView.setSurfaceTextureListener(this);
        mCamera = new Camera2APIs(this);

        mTextureView.setSurfaceTextureListener(this);
        //bluetooth
        /*
         *
         */
        //firebase-setting
        setFirebaseOptions();
        //FirebaseVisionImage image = FirebaseVisionImage.fromBitmap(myBitmap);

        //Handler for current time
//    Handler myTimer = new Handler(){
//        public void handleMessage(Message msg){
//            if (tabCount > 1) {
//                String res = getString(R.string.sales_next_round);
//                res += "(";
//                res += String.valueOf(getSecTime());
//                res += ")";
//                mTextView.setText(res);//print the elapsed time into top board
//                myTimer.sendEmptyMessage(0);    //sendEmptyMessage is now send a null message into Handler
//            }
//        }
//    };
//    long getSecTime()
//    {
//        long now = SystemClock.elapsedRealtime();
//        long outTime = now - baseTime;
//        long seconds = outTime/1000, mins = seconds /60, hours = mins /60;
//        //String hm_outTime = String.format("%02d:%02d:%02d", hours, mins % 60, seconds % 60);
//        //return hm_outTime;
//        return (seconds % 60);
//    }
    }
    public void setFirebaseOptions() {

        // High-accuracy landmark detection and face classification
        highAccuracyOpts =
                new FirebaseVisionFaceDetectorOptions.Builder()
                        .setPerformanceMode(FirebaseVisionFaceDetectorOptions.ACCURATE)
                        .setLandmarkMode(FirebaseVisionFaceDetectorOptions.ALL_LANDMARKS)
                        .setClassificationMode(FirebaseVisionFaceDetectorOptions.ALL_CLASSIFICATIONS)
                        .build();
        // Real-time contour detection of multiple faces
        realTimeOpts = new FirebaseVisionFaceDetectorOptions.Builder()
                .setContourMode(FirebaseVisionFaceDetectorOptions.ALL_CONTOURS)
                .build();
        detector = FirebaseVision.getInstance().getVisionFaceDetector(highAccuracyOpts);
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (mTextureView.isAvailable()) {
            openCamera();
        } else {
            mTextureView.setSurfaceTextureListener(this);
        }
    }
    /* Surface Callbacks */
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int i, int i1) {
        openCamera();
    }
    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int i, int i1) {

    }
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
        return true;
    }
    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {

    }
    private void openCamera() {
        CameraManager cameraManager = mCamera.CameraManager_1(this);
        String cameraId = mCamera.CameraCharacteristics_2(cameraManager);
        mCamera.CameraDevice_3(cameraManager, cameraId);
    }
    @Override
    public void onCameraDeviceOpened(CameraDevice cameraDevice, Size cameraSize) {
        SurfaceTexture texture = mTextureView.getSurfaceTexture();
        texture.setDefaultBufferSize(cameraSize.getWidth(), cameraSize.getHeight());
        Surface surface = new Surface(texture);

        mCamera.CaptureSession_4(cameraDevice, surface);
        mCamera.CaptureRequest_5(cameraDevice, surface);
    }
    private void closeCamera() {
        mCamera.closeCamera();
    }
    @Override
    protected void onPause() {
        closeCamera();
        super.onPause();
    }
    public void setTextInTime(final String msg, long _intime) {
        new Handler().postDelayed(new Runnable()
        {
            @Override
            public void run()
            {
                mTextView.setText(msg);
            }
        }, _intime);
    }
    public void voicePermissionChecker(){
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED
                || ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1);
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_VOICE : {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
                } else {
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                return;
            }
            // other 'case' lines to check for other
            // permissions this app might request
        }
    }
}
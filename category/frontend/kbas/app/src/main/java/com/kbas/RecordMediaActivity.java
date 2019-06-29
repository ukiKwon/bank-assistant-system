package com.kbas;

import android.content.Intent;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.os.Bundle;
import android.util.Size;
import android.view.Surface;
import android.view.TextureView;

import androidx.appcompat.app.AppCompatActivity;

public class RecordMediaActivity extends AppCompatActivity
      implements Camera2APIs.Camera2Interface, TextureView.SurfaceTextureListener {
    //to-saved
    private VisitedData mVisitedData = null;
    //camera
    private TextureView mTextureView;
    private Camera2APIs mCamera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        setConfig();
    }
    public void setConfig() {
        //camera-view
        mTextureView = (TextureView) findViewById(R.id.textureView);
        mTextureView.setSurfaceTextureListener(this);
        mCamera = new Camera2APIs(this);
        //bluetooth
        /*
          *
        */
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
}

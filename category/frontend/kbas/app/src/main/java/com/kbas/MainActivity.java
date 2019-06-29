//socket_code_ref : http://mystudyroom.net/2017/10/11/%EC%95%88%EB%93%9C%EB%A1%9C%EC%9D%B4%EB%93%9C-node-js%EC%84%9C%EB%B2%84%EB%A1%9C-post%EB%B0%A9%EC%8B%9D%EC%9C%BC%EB%A1%9C-%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%A5%BC-%EB%B3%B4%EB%82%B4%EA%B8%B0/

package com.kbas;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;
import android.os.Bundle;
import android.widget.Toast;

import com.airbnb.lottie.LottieAnimationView;
import com.google.gson.Gson;

import org.json.JSONObject;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    //banker-setting
    private final String bankerid = "b0001";
    //aws-commu
    private final String mProtocol = "http://";
    private final String mServerPublicIp = "13.125.216.41";
    private final String mServerPort = "3000";
    private final String[] mServerTargetDir = {"/users", "/post", "/bankque"};
    private final int mServerCount = 3;
    private String mUrls[] = new String[mServerCount];
    private final String[] mDeliver = {"POST", "GET"};
    private final String[] ERROR_STATE = {"10001", "10002"};
    //mainactivity-variables
    private final String TAG = "MainActivity";
    private final String DELIMETER = ":";
    private boolean SERVER_STATE = false;
    private boolean CUSTOM_STATE = false;
    private VisitedData mVisitedData;
    private final int MY_PERMISSIONS_REQUEST_CAMERA = 00001;
    //mainView
    private LottieAnimationView mAnimationView;
    private TextView mServerData;  //서버와 연결 상태 출력
    private TextView mCustomData;//배정받은 번호와 고객 이름 출력

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setConfig();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i("MainActivity", "onResume()");
    }

    public void setConfig() {
        //permission-check
        cameraPermissionChecker();
        //aws-connection
        mUrls[0] = mProtocol + mServerPublicIp + ":" + mServerPort + mServerTargetDir[2];
        new JSONTask().execute(mUrls);//AsyncTask 시작시킴
        //mainView;
        mServerData = (TextView) findViewById(R.id.serverview);
        mCustomData = (TextView) findViewById(R.id.customview);
        mAnimationView = (LottieAnimationView) findViewById(R.id.animation_view);
        //touch-event
        mAnimationView.setOnTouchListener(new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                    case MotionEvent.ACTION_MOVE:
                    case MotionEvent.ACTION_UP:
                        //camera 이벤트 넘어가기
                        if (CUSTOM_STATE && mVisitedData != null) {
                            Intent intent = new Intent(MainActivity.this, RecordMediaActivity.class);
                            intent.putExtra("cid", mVisitedData.getCustomId());
                            intent.putExtra("cname", mVisitedData.getCustomName());
                            startActivity(intent);
                        }
                }
                return true;
            }
        });
    }

    public class JSONTask extends AsyncTask<String, String, String> {
        @Override
        protected String doInBackground(String[] urls) {
            try {
                //JSONObject를 만들고 key value 형식으로 값을 저장해준다.
                JSONObject jsonObject = new JSONObject();
                jsonObject.accumulate("bid", bankerid);
                HttpURLConnection con = null;
                BufferedReader reader = null;
                try {
                    //URL url = new URL(urls[0] + mServerTargetDir);
                    URL url = new URL(urls[0]);
                    //연결을 함
                    con = (HttpURLConnection) url.openConnection();
                    con.setRequestMethod(mDeliver[0]);//POST방식으로 보냄
                    con.setRequestProperty("Cache-Control", "no-cache");//캐시 설정
                    con.setRequestProperty("Content-Type", "application/json");//application JSON 형식으로 전송
                    con.setRequestProperty("Accept", "text/html");//서버에 response 데이터를 html로 받음
                    con.setDoOutput(true);//Outstream으로 post 데이터를 넘겨주겠다는 의미
                    con.setDoInput(true);//Inputstream으로 서버로부터 응답을 받겠다는 의미
                    con.connect();

                    //서버로 보내기위해서 스트림 만듬
                    OutputStream outStream = con.getOutputStream();

                    //버퍼를 생성하고 넣음
                    BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(outStream));
                    writer.write(jsonObject.toString());
                    writer.flush();
                    writer.close();//버퍼를 받아줌

                    //서버로 부터 데이터를 받음
                    InputStream stream = con.getInputStream();
                    reader = new BufferedReader(new InputStreamReader(stream));
                    StringBuffer buffer = new StringBuffer();
                    String line = "";

                    while ((line = reader.readLine()) != null) {
                        buffer.append(line);
                    }
                    return buffer.toString();//서버로 부터 받은 값을 리턴해줌 아마 OK!!가 들어올것임
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (con != null) {
                        con.disconnect();
                    }
                    try {
                        if (reader != null) {
                            reader.close();//버퍼를 닫아줌
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            } catch (Exception e) {

                e.printStackTrace();
            }
            return null;
        }
        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);
            //status check
            CUSTOM_STATE = checkException(result);
            SERVER_STATE = (result != null);
            //init-visited data setting
            String[] res = null;
            if (SERVER_STATE && CUSTOM_STATE) {
                res = result.split(DELIMETER);
                if (res.length >= 2) {
                    System.out.println("id : " + res[0].substring(1));
                    System.out.println("name : " + res[1].substring(0, res[1].length() - 1));
                    mVisitedData = new VisitedData(res[0], res[1]);
                    result = res[1].substring(0, res[1].length() - 1);
                }
            }
            //If the server is on
            setLottieView();
            setTextView(result);
        }
    }
    public void setLottieView() {
        mAnimationView.cancelAnimation();
        mAnimationView.setAnimation(SERVER_STATE & CUSTOM_STATE? R.raw.tab : R.raw.loading);
        mAnimationView.playAnimation();
    }
    public void setTextView(String _custom_name){
        System.out.println("_custom_name : " + _custom_name);
        mCustomData.setText(SERVER_STATE & CUSTOM_STATE? (_custom_name + getString(R.string.found_custom)) : getString(R.string.not_found_custom));
        mServerData.setText(SERVER_STATE & CUSTOM_STATE? getString(R.string.server_connected) : getString(R.string.server_not_connected));
    }
    public boolean checkException(String _custom_name) {
        boolean flag = true;
        for (int i = 0; flag && i < ERROR_STATE.length; ++i )
            flag = (ERROR_STATE[i] == _custom_name);
        return !flag;
    }
    public void cameraPermissionChecker() {
        if (ContextCompat.checkSelfPermission(MainActivity.this,
                Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED) {

            // Should we show an explanation?
            if (ActivityCompat.shouldShowRequestPermissionRationale(MainActivity.this,
                    Manifest.permission.CAMERA)) {

                // Show an expanation to the user *asynchronously* -- don't block
                // this thread waiting for the user's response! After the user
                // sees the explanation, try again to request the permission.

            } else {
                // No explanation needed, we can request the permission.
                ActivityCompat.requestPermissions(MainActivity.this,
                        new String[]{Manifest.permission.CAMERA},
                        MY_PERMISSIONS_REQUEST_CAMERA);

                // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
                // app-defined int constant. The callback method gets the
                // result of the request.
            }
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_CAMERA: {
                // If request is cancelled, the result arrays are empty.
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

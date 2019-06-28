package com.kbas;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

import com.airbnb.lottie.LottieAnimationView;
import com.airbnb.lottie.LottieDrawable;

import androidx.appcompat.app.AppCompatActivity;

public class IntroActivity extends AppCompatActivity {
    private LottieAnimationView animationView;
    private LottieDrawable lottieDrawable;
    private long start_time, wait_time;
    private final int INTRO_CYCLE_TIME = 4000;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_intro);
        setConfig();
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Intent welcome = new Intent( IntroActivity.this, MainActivity.class);
                startActivity(welcome);
            }
        }, INTRO_CYCLE_TIME);
    }
    public void setConfig() {
//        start_time = System.currentTimeMillis();
//        animationView = (LottieAnimationView) findViewById(R.id.animation_view);
//
//        animationView.addAnimatorUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
//            @Override
//            public void onAnimationUpdate(ValueAnimator valueAnimator) {
//                // Do something.
//                wait_time = System.currentTimeMillis();
//                //ToDo: 서버 연결되면, 넘어가도록 구현할 것
//                //System.out.println("wait_time - start_time = " + (wait_time - start_time) );
//                if (wait_time - start_time >= INTRO_CYCLE_TIME) {
//                    Intent server_return = new Intent(IntroActivity.this, MainActivity.class);
//                    startActivity(server_return);
//                }
//            }
//        });
//        animationView.playAnimation();
//        if (animationView.isAnimating()) {
//            // Do something.
//            System.out.println(">> Lottie is on");
//        }
//        animationView.setProgress(0.5f);
    }
    @Override
    public void onPause() {
        super.onPause();
        // Remove the activity when its off the screen
        finish();
    }
}

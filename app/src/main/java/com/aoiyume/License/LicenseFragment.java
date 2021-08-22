package com.aoiyume.License;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.R.drawable;

import com.aoiyume.monsterfriends.MainActivity;
import com.aoiyume.monsterfriends.R;

import java.io.IOException;

import androidx.activity.OnBackPressedCallback;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

/**
 * Created by 葵ユメ on 2021/08/22.
 */

public class LicenseFragment extends Fragment {
    enum BackButtonState {
        eDOWN, eKeep, eUP
    }
    final float ANIM_Y_OFFSET = -128.0f;
    final float ANIM_Y_SPEED = 3.0f;
    BackButtonState m_BackButtonState = BackButtonState.eUP;
    ImageButton m_BackButton = null;
    Thread m_Thread = null;
    int m_BackButtonKeep = 0;
    boolean m_ThreadLoop = true;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.license_layout, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        WebView webView = view.findViewById(R.id.webView);
        webView.loadUrl("file:///android_asset/licenses.html");
        webView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                int action = motionEvent.getAction();
                if(action == MotionEvent.ACTION_MOVE) {
                    if(m_BackButtonState != BackButtonState.eDOWN) {
                        m_BackButtonState = BackButtonState.eDOWN;
                    }
                    return false;
                }
                return false;
            }
        });

        m_BackButton = new ImageButton(MainActivity.GetContext());
        m_BackButton.setImageResource(drawable.ic_menu_revert);
        m_BackButton.setBackgroundColor(0x88000000);
        m_BackButton.setContentDescription("Back Button");
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        ((ViewGroup.MarginLayoutParams)params).setMargins(16, 16, 0, 0);
        m_BackButton.setY(ANIM_Y_OFFSET);
        m_BackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                MainActivity.GetContext().getSupportFragmentManager().popBackStack();
            }
        });

        ViewGroup parent = (ViewGroup) webView.getParent();
        parent.addView(m_BackButton, params);


        m_Thread = new Thread(new Runnable() {
            @Override
            public void run() {
                while(m_ThreadLoop) {
                    m_BackButton.post(new Runnable() {
                        @Override
                        public void run() {
                            if(m_BackButtonState == BackButtonState.eDOWN) {
                                float y = m_BackButton.getY();
                                y = Math.min(y + ANIM_Y_SPEED, 0.0f);
                                m_BackButton.setY(y);

                                m_BackButtonKeep = 0;
                                if(y >= 0.0f) { m_BackButtonState = BackButtonState.eKeep; }
                            }
                            else if(m_BackButtonState == BackButtonState.eKeep) {
                                m_BackButtonKeep++;
                                if(m_BackButtonKeep >= 120) { m_BackButtonState = BackButtonState.eUP; }
                            }
                            else if(m_BackButtonState == BackButtonState.eUP) {
                                float y = m_BackButton.getY();
                                y = Math.max(y - ANIM_Y_SPEED, ANIM_Y_OFFSET);
                                m_BackButton.setY(y);

                            }
                        }
                    });
                    try {
                        Thread.sleep(16);
                    } catch(InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        m_Thread.start();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        try {
            m_ThreadLoop = false;
            m_Thread.join();
        }catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

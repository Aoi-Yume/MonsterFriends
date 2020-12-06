package com.aoiyume.Game;

import android.os.Bundle;
import android.os.Debug;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

import com.aoiyume.monsterfriends.MainActivity;
import com.aoiyume.monsterfriends.R;

import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;

/**
 * Created by 葵ユメ on 2018/08/13.
 */

public class GameMainFragment extends Fragment {
    static boolean m_bShowKeyboard = false;
    static EditText m_InputText = null;

    static Runnable ShowSoftKeyborad = new Runnable() {
        @Override
        public void run() {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            InputMethodManager imm = (InputMethodManager)MainActivity.GetContext().getSystemService(MainActivity.GetContext().INPUT_METHOD_SERVICE);

            if(imm != null) {
                imm.showSoftInput(m_InputText, 0);
            }
        }
    };

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.game_main_layout, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        //view.setFocusable(true);
        //view.setFocusableInTouchMode(true);
        //view.requestFocus();

        m_InputText = view.findViewById(R.id.inputText);
        m_InputText.setImeOptions(EditorInfo.IME_ACTION_DONE);
        m_InputText.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View view, boolean b) {
                if(b) {
                    new Handler(Looper.getMainLooper()).post(ShowSoftKeyborad);
                }
                else {
                    m_bShowKeyboard = false;
                    InputMethodManager imm = (InputMethodManager)MainActivity.GetContext().getSystemService(MainActivity.GetContext().INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
                }
            }
        });
        m_InputText.setOnEditorActionListener(new TextView.OnEditorActionListener(){
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                boolean bEntorUp = false;
                if(actionId == EditorInfo.IME_ACTION_NONE) {
                    if (keyEvent != null && keyEvent.getAction() == KeyEvent.ACTION_UP) {
                        bEntorUp = true;
                    }
                }
                if(actionId == EditorInfo.IME_ACTION_DONE || bEntorUp) {
                    m_bShowKeyboard = false;
                    InputMethodManager imm = (InputMethodManager)MainActivity.GetContext().getSystemService(MainActivity.GetContext().INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(textView.getWindowToken(), 0);
                    return true;
                }
                return false;
            }
        });

        //Log.d("Monster Friends", "result:" + Boolean.toString(result));

        /*
        ImageButton titleButton = (ImageButton) view.findViewById(R.id.TitleButtton);
        titleButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FragmentManager mgr = ((MainActivity)MainActivity.GetContext()).getSupportFragmentManager();
                FragmentTransaction transaction =  mgr.beginTransaction();

                transaction.replace(R.id.container, new ButtonFragment());
                transaction.commit();
            }
        });
        */
    }

    public static void ShowSoftwareKeyboard()
    {
        Log.d("Monster Friends", "call ShowSoftwareKeyboard\n");
        m_bShowKeyboard = true;
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                m_InputText.requestFocus();
            }
        });
    }

    public static boolean IsShowSoftwareKeyboard()
    {
        return m_bShowKeyboard;
    }
    public static String GetInputText() { return m_InputText.getText().toString(); }
}

package com.aoiyume.monsterfriends;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

/**
 * Created by 葵ユメ on 2020/11/03.
 */

public class LoginFragment extends Fragment {
    private EditText m_UserNameEdit;
    private TextView m_GroupList;
    private Button m_JoinButton;
    private Button m_StartButton;

    private final View.OnFocusChangeListener onUserNameFocus =
            new View.OnFocusChangeListener() {
                @Override
                public void onFocusChange(View view, boolean isFocus) {
                    String text = m_UserNameEdit.getText().toString();
                    if (!isFocus) {
                        if(text.isEmpty()) {
                            m_UserNameEdit.setText("Name");
                        }
                        return;
                    }
                    if (text.matches("Name")) {
                        m_UserNameEdit.setText("");
                    }
                }
            };

    // Fragmentで表示するViewを作成するメソッド
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        super.onCreateView(inflater, container, savedInstanceState);
        // 先ほどのレイアウトをここでViewとして作成します
        return inflater.inflate(R.layout.login_fragment, container, false);
    }

    // Viewが生成し終わった時に呼ばれるメソッド
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        m_UserNameEdit = view.findViewById(R.id.UserName);
        m_GroupList = view.findViewById(R.id.GroupList);
        m_JoinButton = view.findViewById(R.id.JoinButton);
        m_StartButton = view.findViewById(R.id.StartButton);

        final MainActivity activity = MainActivity.GetContext();
        m_UserNameEdit.setOnFocusChangeListener(onUserNameFocus);

        m_StartButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TalkFragment fragment = new TalkFragment();
                FragmentTransaction transaction = activity.getSupportFragmentManager().beginTransaction();
                transaction.replace(R.id.container, fragment);
                transaction.commit();
            }
        });
    }
}

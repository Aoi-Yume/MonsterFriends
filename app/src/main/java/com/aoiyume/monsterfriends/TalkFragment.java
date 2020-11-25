package com.aoiyume.monsterfriends;

import android.graphics.Point;
import android.os.Bundle;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.fragment.app.Fragment;

import com.aoiyume.Game.Utility;

/**
 * Created by 葵ユメ on 2020/11/03.
 */

public class TalkFragment extends Fragment {
    private TextView m_TalkView;
    private EditText m_SendText;
    private Button m_SendButton;

    // Fragmentで表示するViewを作成するメソッド
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        super.onCreateView(inflater, container, savedInstanceState);
        // 先ほどのレイアウトをここでViewとして作成します
        return inflater.inflate(R.layout.talk_fragment, container, false);
    }

    // Viewが生成し終わった時に呼ばれるメソッド
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        m_TalkView = view.findViewById(R.id.talkView);
        m_TalkView.setHeight((int)(Utility.GetApplicationScreenSize().y * 0.7f));
        m_SendText = view.findViewById(R.id.SendText);
        m_SendButton = view.findViewById(R.id.SendButton);

        m_SendButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        MainActivity.GetContext().GetNearbyClient().SendData(m_SendText.getText().toString());
                    }
                }
        );

        MainActivity.GetContext().GetNearbyClient().SetMessageCallBack(
                new MessageReceiveCallBack() {
                    @Override
                    public void receive(String endPointId, String str) {
                        m_TalkView.append(endPointId + ":" + str + "\n");
                    }
                }
        );
    }
}

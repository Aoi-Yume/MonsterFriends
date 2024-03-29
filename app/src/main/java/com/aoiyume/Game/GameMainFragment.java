package com.aoiyume.Game;

import android.content.pm.PackageManager;
import android.media.Ringtone;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.aoiyume.License.LicenseFragment;
import com.aoiyume.monsterfriends.MainActivity;
import com.aoiyume.monsterfriends.R;
import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;

import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

/**
 * Created by 葵ユメ on 2018/08/13.
 */

public class GameMainFragment extends Fragment {
    static boolean m_bShowKeyboard = false;
    static EditText m_InputText[] = null;
    static int m_CurrentInpuTextNo = 0;
    static final int m_InputTextMax = 2;
    static NearbyClient m_NearbyClient = null;
    static InterstitialAd m_InterstitialAd = null;
    static boolean m_LoadFailedAd = false;

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
                imm.showSoftInput(m_InputText[m_CurrentInpuTextNo], 0);
            }
        }
    };

    static ActivityResultLauncher<String> m_requestPermissionLauncher = null;
    enum PermissionResult {
        eNone,
        eSuccess,
        eFailed,
        eDetailExplain,
    }
    static PermissionResult m_PermissionResult = PermissionResult.eNone;

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

        m_CurrentInpuTextNo = 0;
        m_InputText = new EditText[m_InputTextMax];
        m_InputText[0] = view.findViewById(R.id.inputText1);
        m_InputText[1] = view.findViewById(R.id.inputText2);
        for(int i = 0; i < m_InputTextMax; ++i) {
            m_InputText[i].setImeOptions(EditorInfo.IME_ACTION_DONE);
            m_InputText[i].setOnFocusChangeListener(new View.OnFocusChangeListener() {
                @Override
                public void onFocusChange(View view, boolean b) {
                    if (b) {
                        new Handler(Looper.getMainLooper()).post(ShowSoftKeyborad);
                    } else {
                        InputMethodManager imm = (InputMethodManager) MainActivity.GetContext().getSystemService(MainActivity.GetContext().INPUT_METHOD_SERVICE);
                        imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
                    }
                }
            });
            m_InputText[i].setOnEditorActionListener(new TextView.OnEditorActionListener() {
                @Override
                public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                    boolean bEntorUp = false;
                    if (actionId == EditorInfo.IME_ACTION_NONE) {
                        if (keyEvent != null && keyEvent.getAction() == KeyEvent.ACTION_UP) {
                            bEntorUp = true;
                        }
                    }
                    if (actionId == EditorInfo.IME_ACTION_DONE || bEntorUp) {
                        m_bShowKeyboard = false;
                        InputMethodManager imm = (InputMethodManager) MainActivity.GetContext().getSystemService(MainActivity.GetContext().INPUT_METHOD_SERVICE);
                        imm.hideSoftInputFromWindow(textView.getWindowToken(), 0);
                        return true;
                    }
                    return false;
                }
            });
        }

        m_NearbyClient = new NearbyClient();
        m_NearbyClient.Initialize();
        m_NearbyClient.SetReceiveCallBack(new ReceiveCallBack() {
            @Override
            public void accept(String endPointId, String Name) {
                Engine.AccecpNearEndPoint(endPointId, Name);
            }

            @Override
            public void receive(String endPointId, byte[] data) {
                Engine.ReceiveNearbyData(endPointId, data);
            }
        });

        m_requestPermissionLauncher = MainActivity.GetContext().registerForActivityResult(
                new ActivityResultContracts.RequestPermission(), new ActivityResultCallback<Boolean>() {
                    @Override
                    public void onActivityResult(Boolean result) {
                        m_PermissionResult = result ? PermissionResult.eSuccess : PermissionResult.eFailed;
                    }
                });
    }

    @Override
    public void onStop() {
        super.onStop();
        Engine.ActivityStop();
    }

    @Override
    public void onResume() {
        super.onResume();
        Engine.ActivityResume();
    }

    public static void ShowSoftwareKeyboard()
    {
        Log.d("Monster Friends", "call ShowSoftwareKeyboard\n");
        m_bShowKeyboard = true;
        m_CurrentInpuTextNo = (m_CurrentInpuTextNo + 1) % m_InputTextMax;
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                m_InputText[m_CurrentInpuTextNo].requestFocus();
            }
        });
    }

    public static boolean IsShowSoftwareKeyboard()
    {
        return m_bShowKeyboard;
    }
    public static String GetInputText() { return m_InputText[m_CurrentInpuTextNo].getText().toString(); }

    public static boolean IsPermissionGranted(String permission)
    {
        MainActivity context = MainActivity.GetContext();
        PackageManager packageManager = context.getPackageManager();
        int nPermission = packageManager.checkPermission(permission,  context.getPackageName());
        return nPermission == PackageManager.PERMISSION_GRANTED;
    }

    public static void CheckPermission(String permission)
    {
        MainActivity context = MainActivity.GetContext();

        m_PermissionResult = PermissionResult.eNone;
        if(IsPermissionGranted(permission)) {
            m_PermissionResult = PermissionResult.eSuccess;
        }
        else {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                if(context.shouldShowRequestPermissionRationale(permission)) {
                    // アプリ独自の詳細な説明
                    m_PermissionResult = PermissionResult.eDetailExplain;
                }
                else {
                   RequestPermission(permission);
                }
            }
            else {
                RequestPermission(permission);
            }
        }
    }

    public static void RequestPermission(String permission)
    {
        m_PermissionResult = PermissionResult.eNone;
        m_requestPermissionLauncher.launch(permission);
    }

    public static int GetPermissionResult()
    {
        return m_PermissionResult.ordinal();
    }

    public static void StartNearbyAdvertising(String ConnectName)
    {
        m_NearbyClient.SetConnectName(ConnectName);
        m_NearbyClient.StartAdvertising();
    }
    public static void StopNearbyAdvertising()
    {
        m_NearbyClient.StopAdvertising();
    }
    public static void StartNearbyDiscovery(String ConnectName)
    {
        m_NearbyClient.SetConnectName(ConnectName);
        m_NearbyClient.StartDiscovery();
    }
    public static void StopNearbyDiscovery()
    {
        m_NearbyClient.StopDiscovery();
    }
    public static void DisconnectNearbyAllEndPoint(){ m_NearbyClient.DisconnectAllEndPoint(); }
    public static void SendData(String Id, byte[] data)
    {
        m_NearbyClient.SendData(Id, data);
    }

    public static void LoadAds()
    {
        m_LoadFailedAd = false;
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                // TODO 製品版では正規のユニットIDを使用する
                InterstitialAd.load(
                        MainActivity.GetContext(), "ca-app-pub-3940256099942544/1033173712",
                        new AdRequest.Builder().build(), new InterstitialAdLoadCallback() {
                            @Override
                            public void onAdLoaded(@NonNull InterstitialAd interstitialAd) {
                                super.onAdLoaded(interstitialAd);
                                m_InterstitialAd = interstitialAd;
                                m_InterstitialAd.setFullScreenContentCallback(new FullScreenContentCallback() {
                                    @Override
                                    public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
                                        super.onAdFailedToShowFullScreenContent(adError);
                                        m_InterstitialAd = null;
                                    }

                                    @Override
                                    public void onAdDismissedFullScreenContent() {
                                        super.onAdDismissedFullScreenContent();
                                        m_InterstitialAd = null;
                                    }
                                });
                            }

                            @Override
                            public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                                m_LoadFailedAd = true;
                                Toast.makeText(MainActivity.GetContext(), "Failed Load Ad", Toast.LENGTH_SHORT).show();
                                super.onAdFailedToLoad(loadAdError);
                            }
                        });
            }
        });
    }
    public static void ShowAds()
    {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                if(m_LoadFailedAd){ return; }
                m_InterstitialAd.show(MainActivity.GetContext());
            }
        });
    }

    public static boolean IsShowAds()
    {
        return m_InterstitialAd != null;
    }

    public static void ShowLicense() {
        LicenseFragment fragment = new LicenseFragment();
        FragmentTransaction transaction = MainActivity.GetContext().getSupportFragmentManager().beginTransaction();
        transaction.add(R.id.container, fragment, "License");
        transaction.addToBackStack(null);
        transaction.commit();
    }

    public static boolean IsShowLicense() {
        return MainActivity.GetContext().getSupportFragmentManager().findFragmentByTag("License") != null;
    }
}

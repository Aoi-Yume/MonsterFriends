package com.aoiyume.Game;

import android.util.Log;
import android.widget.Toast;

import com.aoiyume.monsterfriends.MainActivity;
import com.google.android.gms.nearby.Nearby;
import com.google.android.gms.nearby.connection.AdvertisingOptions;
import com.google.android.gms.nearby.connection.ConnectionInfo;
import com.google.android.gms.nearby.connection.ConnectionLifecycleCallback;
import com.google.android.gms.nearby.connection.ConnectionResolution;
import com.google.android.gms.nearby.connection.ConnectionsClient;
import com.google.android.gms.nearby.connection.DiscoveredEndpointInfo;
import com.google.android.gms.nearby.connection.DiscoveryOptions;
import com.google.android.gms.nearby.connection.EndpointDiscoveryCallback;
import com.google.android.gms.nearby.connection.Payload;
import com.google.android.gms.nearby.connection.PayloadCallback;
import com.google.android.gms.nearby.connection.PayloadTransferUpdate;
import com.google.android.gms.nearby.connection.Strategy;

import androidx.annotation.NonNull;

/**
 * Created by 葵ユメ on 2020/11/03.
 */

interface ReceiveCallBack{
    void accept(String endPointId, String Name);
    void receive(String endPointId, byte[] data);
}

public class NearbyClient {
    private static final Strategy STRATEGY = Strategy.P2P_STAR;
    private static final String TAG = "MonsterFriends";

    // Our handle to Nearby Connections
    private ConnectionsClient m_ConnectionsClient;
    private String m_ConnectName;

    private  ReceiveCallBack m_ReceiveCallback;

    // Callbacks for finding other devices
    private final EndpointDiscoveryCallback endpointDiscoveryCallback =
            new EndpointDiscoveryCallback() {
                @Override
                public void onEndpointFound(String endpointId, DiscoveredEndpointInfo info) {
//                    Log.i(TAG, "onEndpointFound: endpoint found, connecting");
                    Toast.makeText(MainActivity.GetContext(), "onEndpointFound: endpoint found, connecting", Toast.LENGTH_SHORT).show();
                    m_ConnectionsClient.requestConnection(m_ConnectName, endpointId, connectionLifecycleCallback);
                }

                @Override
                public void onEndpointLost(String endpointId) {}
            };

    private  final PayloadCallback payloadCallback =
            new PayloadCallback() {
                @Override
                public void onPayloadReceived(@NonNull String endPointId, @NonNull Payload payload) {
                    if(payload.getType() != Payload.Type.BYTES){ return; }
                    m_ReceiveCallback.receive(endPointId, payload.asBytes());
                }

                @Override
                public void onPayloadTransferUpdate(@NonNull String endPointId, @NonNull PayloadTransferUpdate payloadTransferUpdate) {

                }
            };

    // Callbacks for connections to other devices
    private final ConnectionLifecycleCallback connectionLifecycleCallback =
            new ConnectionLifecycleCallback() {
                @Override
                public void onConnectionInitiated(String endpointId, ConnectionInfo connectionInfo) {
                    Toast.makeText(MainActivity.GetContext(), "onConnectionInitiated: accepting connection", Toast.LENGTH_SHORT).show();
                    Log.i(TAG, "onConnectionInitiated: accepting connection");
                    m_ConnectionsClient.acceptConnection(endpointId, payloadCallback);
                    m_ReceiveCallback.accept(endpointId, connectionInfo.getEndpointName());
                }

                @Override
                public void onConnectionResult(String endpointId, ConnectionResolution result) {
                    if (result.getStatus().isSuccess()) {
                        Toast.makeText(MainActivity.GetContext(), "onConnectionResult: connection successful", Toast.LENGTH_SHORT).show();
                        Log.i(TAG, "onConnectionResult: connection successful");
                    } else {
                        Toast.makeText(MainActivity.GetContext(), "onConnectionResult: connection failed", Toast.LENGTH_SHORT).show();
                    }
                }

                @Override
                public void onDisconnected(String endpointId) {
                    Toast.makeText(MainActivity.GetContext(), "onDisconnected: disconnected from the opponent", Toast.LENGTH_SHORT).show();
//                    Log.i(TAG, "onDisconnected: disconnected from the opponent");
//                    resetGame();
                }
            };

    public void Initialize() {
        m_ConnectionsClient = Nearby.getConnectionsClient(MainActivity.GetContext());
    }

    public void SetConnectName(String connectName){
        m_ConnectName = connectName;
    }

    public void SetReceiveCallBack(ReceiveCallBack callback)
    {
        m_ReceiveCallback = callback;
    }

    /** Starts looking for other players using Nearby Connections. */
    public void StartDiscovery() {
        // Note: Discovery may fail. To keep this demo simple, we don't handle failures.
        m_ConnectionsClient.startDiscovery(
                MainActivity.GetContext().getPackageName(), endpointDiscoveryCallback,
                new DiscoveryOptions.Builder().setStrategy(STRATEGY).build());
    }

    public void StopDiscovery() {
        m_ConnectionsClient.stopDiscovery();
    }

    /** Broadcasts our presence using Nearby Connections so other players can find us. */
    public void StartAdvertising() {
        // Note: Advertising may fail. To keep this demo simple, we don't handle failures.
        m_ConnectionsClient.startAdvertising(
                m_ConnectName, MainActivity.GetContext().getPackageName(),
                connectionLifecycleCallback,
                new AdvertisingOptions.Builder().setStrategy(STRATEGY).build());
    }

    public void StopAdvertising() {
        m_ConnectionsClient.stopAdvertising();
    }

    public void SendData(String Id, byte[] data)
    {
        m_ConnectionsClient.sendPayload(Id, Payload.fromBytes(data));
    }
}

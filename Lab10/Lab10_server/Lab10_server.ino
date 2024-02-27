#include <painlessMesh.h>

#define   MESH_PREFIX     "-"  // name of the mesh
#define   MESH_PASSWORD   "-"
#define   MESH_PORT       5555
#define   IS_ROOT         true          // root node
//#define   IS_ROOT         false          // each mesh contains only one root node
#define   HOSTNAME        "IoTPlat"

String nodeName = "-";  // name of this esp32 board

Scheduler userScheduler;
painlessMesh  mesh;

// wifi 
#define   STATION_SSID     "-"
#define   STATION_PASSWORD "-"
#define   STATION_PORT     5555

IPAddress myAPIP(0,0,0,0); // set IP address of AP mode
IPAddress myIP(0,0,0,0);  // 定義存放本機IP的變數，同時預設為 0,0,0,0
IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());  //傳回本機IP Address
}

void sendMsg();

Task taskSendMsg( TASK_SECOND * 1 , TASK_FOREVER, sendMsg );

void sendMsg() {
  String msg = "我是" + nodeName + "，ID: " + mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMsg.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("收到來自 %u 的訊息：%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void setup() {
  Serial.begin(115200);
  
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  // Channel set to 6 (max. 10 in ESP32). Make sure to use the same channel for your mesh and for you other
  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);

  userScheduler.addTask( taskSendMsg ); // add "taskSendMsg" to scheduler
  // taskSendMsg.enable(); 
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);  //手動連接wifi
  //mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT); //指定port

  // Bridge node, should (in most cases) be a root node. 
  // See [the wiki]
  // (https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(IS_ROOT);        // set this node as root of this mesh
  mesh.setContainsRoot(true);   // it is required that this mesh must contain root 

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("IP address of AP mode:" + myAPIP.toString());
}

void loop() {
  mesh.update(); //update task

  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("Sever IP address" + myIP.toString());
  }
}
#include <painlessMesh.h>

#define   MESH_PREFIX     "-"  // name of the mesh
#define   MESH_PASSWORD   "-"
#define   MESH_PORT       5555
#define   IS_ROOT         false

String nodeName = "-";
Scheduler userScheduler;
painlessMesh  mesh;

int trig = 12;
int echo = 11;
int dua,dis,current,prev;

void sendMsg();

Task taskSendMsg( TASK_SECOND * 1 , TASK_FOREVER, sendMsg );

void sendMsg() {
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dua = pulseIn(echo,HIGH);
  dis = dua * 0.034 / 2;

  // String msg = "我是" + nodeName + "，ID: " + mesh.getNodeId() + "超聲波距離是: " + dis;  
  String msg = "我是" + nodeName + "超聲波距離是: " + dis;

  if(dis<10){
    msg+=" 距離太近!!";
  }
  mesh.sendBroadcast( msg );
  taskSendMsg.setInterval( TASK_SECOND * 3);
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("收到來自 %u 的訊息：%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("新的連線，nodeId: %u\n", nodeId);
  Serial.printf("新的連線，JSON結構:\n%s\n", mesh.subConnectionJson(true).c_str());
}

void setup() {
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(115200);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA );
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection(newConnectionCallback);
  mesh.setRoot(IS_ROOT);
  mesh.setContainsRoot(true);

  userScheduler.addTask( taskSendMsg );
  taskSendMsg.enable();
  Serial.printf("我是%s，ID：%u\n", nodeName, mesh.getNodeId());
}

void loop() {
  mesh.update();
}
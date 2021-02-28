<template>
  <q-page class="column content-center q-ma-sm">
    <!-- Login -->
    <template v-if="getPage=='inicio'">
      <div>
        <div class="q-my-sm text-center">
          <q-img src='../statics/logo2.png' height="100%" width="80%" />
        </div>
        <div class="q-mt-xl q-my-sm">
          <span class="text-h4">Seja bem vindo!</span>
          <q-input class="q-my-sm" v-model="userName" type="text" placeholder="Insira seu nome aqui" />
          <q-btn class="full-width" align="center" color="primary" label="ENTRAR" @click="login" />
        </div>
      </div>
    </template>
    <!-- Paginal Inicial -->
    <template v-else-if="getPage=='pageOne'">
      <div v-if="devices&&devices.length>0" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Lista de dispositivos disponíveis/pareados -->
      <q-list class="full-width q-ma-sm" bordered  v-for="(device, index) in devices" :key="index">
        <q-btn class="full-width" color="primary" :label="dispConectado ? 'Conectado' : 'Conectar'" @click="conectarDispositivo(device)" :disable="dispConectado ? true : false"/>
        <q-item >
          <q-item-section class="col-2" avatar>
            <q-icon color="primary" name="light" />
          </q-item-section>
          <q-item-section class="col-8">
            <span class="text-h6"> {{device.name}}<span style="font-size:13px;">{{device.id ? ' - '+device.id : ''}}</span> </span>
            <div v-if="device.comodos" class="full-width row">
              <div class="col-12 q-mt-md" v-for="(comodo, index) in device.comodos" :key="index">
                <span style="font-size:1.0rem" class="text-bold">Comodo {{index}} - {{comodo.name}}</span>
                <div class="col-12"><q-btn class="full-width" color="secondary" icon="power" label="Acionar" @click="enviarDado(device,'acionar|'+(index+1))" :loading="loading" :disable="dispConectado ? false : true"/></div>
              </div>
            </div>
          </q-item-section>
          <q-item-section  avatar class="col-2">
            <q-icon size="8vw" v-ripple color="secondary" name="settings" @click="editarComodos=device.comodos||[];editarEquipamento=device;editarEquipamentoModal=true" />
          </q-item-section>
        </q-item>
      </q-list>
      <div v-if="comandoVoz" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Mostrar comando de voz enviado -->
      <div class="full-width">
        <div v-if="comandoVoz" class="full-width">
          <span  class="text-h8 text-primary">Comando enviado: </span>
        </div>
        <div v-if="comandoVoz" class="full-width text-center">
          <span style="font-size:1.2rem">{{comandoVoz[0]}}<br></span>
        </div>
      </div>
      <div v-if="comandoVoz" class="full-width q-my-xs">
        <q-separator color="secondary" />
      </div>
      <!-- Adicionar Dispositivo -->
      <div class="q-mt-xl" :class="devices&&devices.length==0 ? 'fixed-center': ''">
        <q-btn class="full-width" color="green-8" icon="add" label="Adicionar Dispositivo" @click="abrirBluetooth"/>
      </div>
      <!-- Acionar comando de voz -->
      <div v-if="devices&&devices.length>0" class="row fixed-bottom full-width justify-center">
        <q-btn round :class="listening ? 'pulse-button' : ''" align="center" class="q-mb-md" color="primary" icon="mic" :size="listening?'8vw':'6vw'" @click="listening? StopSpeechRecognition():StartSpeechRecognition()" :disable="dispConectado ? false : true"/>
      </div>
    </template>
    <!-- Configuração de dispositivo / Bluetooh -->
    <template v-else-if="getPage=='bluetooth'">
      <div class="columns full-width flex-center">
        <div class="q-ma-lg">
          Estado do Bluetooth: <strong>{{getBluetoothState ? 'Ligado' : 'Desligado'}}</strong>
        </div>
        <q-separator class="q-mb-sm" color="black" />
        <div>
          <q-btn v-if="getBluetoothState" class="full-width q-ma-md" color="primary" icon="search" label="Procurar dispositivo" @click="procurarDispositivos" />
          <q-btn v-if="dispositivos&&dispositivos.length==0 && getBluetoothState" class="full-width q-ma-md" color="primary" label="Carregar lista de dispositivos pareados" @click="dispositivosPareados" />
        </div>
        <q-separator class="q-mb-sm" color="black" />
        <div class="q-ma-md" v-if="dispositivos&&dispositivos.length!=0">
          <span class="text-bold q-mb-md">Dispositivos disponíveis: </span>
          <q-list bordered v-for="(device, index) in dispositivos" :key="index">
            <q-item v-ripple >
              <q-item-section avatar>
                <q-btn class="full-width" color="primary" icon="bluetooth_connected"  @click="conectarDispositivo(device,  true)"/>
              </q-item-section>
              <q-item-section class="columns">
                <q-input v-model="device.name" type="text" />
                <span>Mac: {{device.id}}</span>
              </q-item-section>
            </q-item>
          </q-list>
        </div>
      </div>
    </template>
    <!-- Definições de equipamento -->
    <q-dialog v-model="editarEquipamentoModal" maximized>
      <q-card>
        <q-card-section class="columns items-center">
          <span class="text-h6 text-bold">Configuração de equipamento</span><br>
          <span class="text-h8">Id: {{editarEquipamento.id}}</span>
        </q-card-section>
        <q-card-section class="full-width">
          <q-separator color="secondary" />
        </q-card-section>
        <q-card-section>
          <q-input class="text-h6" color="primary" v-model="editarEquipamento.name" type="text" label="Nome do Equipamento">
            <q-btn flat color="primary" icon="clear" size="4vw" @click="editarEquipamento.name=''" />
          </q-input>
        </q-card-section>
        <q-card-section>
          <div class="ful-width" v-for="(comodo, index) in editarComodos" :key="index">
            <q-input class="text-h6" color="primary" v-model="comodo.name" type="text" label="Nome do Comodo">
              <q-btn flat color="primary" icon="delete" size="4vw" @click="excluirComodo(comodo)" />
            </q-input>
          </div>
        </q-card-section>
        <q-card-section v-if="!editarComodos || editarComodos.length < 3">
          <q-btn class="full-width" color="primary" icon="add" label="Adicionar comodo" @click="editarComodos.push({name:''})" />
        </q-card-section>
        <q-card-section class="q-mt-xl">
          <q-btn class="full-width" color="red-10" icon="delete" label="Deletar Dispositivo" @click="excluirDipositivo(editarEquipamento)" />
        </q-card-section>
        <q-card-actions align="around" class="" style="bottom:0px">
          <q-separator class="full-width q-mb-sm" color="secondary" />
          <q-btn style="width:40%" label="Cancelar" color="red-8" v-close-popup />
          <q-btn style="width:40%" label="Salvar" color="green-8" v-close-popup @click="salvarAlteracoesEquipamento(editarEquipamento, editarComodos)"/>
        </q-card-actions>
      </q-card>
    </q-dialog>
  </q-page>
</template>

<script>
/* eslint-disable */

import { Dialog, Loading, Notify } from 'quasar'

const option = {
  language:"pt-BR",
  matches:5,
  prompt:"",      // Android only
  showPopup:false,  // Android only
  showPartial:false 
}

export default {
  name: 'PageIndex',
  data () {
    return {
      text: '',
      luzes: [],
      dispositivos: [],
      dispConectado: '',
      comandoVoz:'',
      listening:false,
      userName:'',
      editarEquipamentoModal:false,
      editarEquipamento:{},
      editarComodos:[],
      loading:false
    }
  },
  created() {
    this.userName = this.$store.getters.getUserName
    if (this.userName && this.$store.getters.getAutoLogin) this.mudarTela('pageOne')
  },
  mounted () {
    this.getLuzes()
  },
  computed: {
    devices () {
      return this.$store.getters.getDevice
    },
    logo () {
      return ''
    },
    getPage () {
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      return this.$store.getters.getPage || 'inicio'
    },
    getBluetoothState() {
      return this.$store.getters.getBluetoothState || false
    }
  },
  methods: {
    // ****************** LOGIN *************************
    login(){
      if(this.userName){
        this.mudarTela('pageOne') 
        this.$store.commit('setUserName', this.userName)
        this.$store.commit('setAutoLogin', true)
      }else {
        Notify.create({message:"Insira um nome.", type:"warning"})
      }
    },

    // ****************** SPEECH RECOGNITION *************************
    StartSpeechRecognition(){
      console.log('Recording')
      this.listening = true
      window.plugins.speechRecognition.startListening(
        (success)=>{
          this.comandoVoz = success
          this.listening = false
          this.acionarDispositivo(success[0].toLowerCase())
        },
        ()=>{console.log("Error on recognizing"); this.listening=false},
        option
      )
    },
    StopSpeechRecognition(){
      window.plugins.speechRecognition.stopListening(
        (success)=>{console.log("Recognition stopped");},
        ()=>{console.log("Error on stop recognition");},
      )
      this.listening=false
    },
    async acionarDispositivo(comando){
      comando = comando.split(" ")
      let action = comando.length>1 ? comando[0] : comando
      let dispName = comando.length>=2 ? comando.length>2 ? comando.slice(1).toString().replaceAll(","," ") : comando[1] : null
      if (dispName && dispName!="") action += "|"+this.getDeviceByName(dispName)
      if (!dispName) {alert("Nenhum dispositivo encontrado");return}
      else this.enviarDado(this.dispConectado, action)
    },
    // ****************** BLEUTOOTH *************************
    abrirBluetooth () {
      bluetoothSerial.isEnabled(
        () => { this.$store.commit('setBluetoothState', true) },
        () => {
          if (this.$q.platform.is.android) {
            bluetoothSerial.enable(
              () => { this.$store.commit('setBluetoothState', true) },
              () => { this.$store.commit('setBluetoothState', false); this.$store.commit('setPage', 'pageOne') }
            )
          } else {
            alert('Ative o Bluetooth')
            this.$store.commit('setPage', 'pageOne')
          }
        }
      )
      this.$store.commit('setPage', 'bluetooth')
    },
    dispositivosPareados(){
      bluetoothSerial.list(
        (devices)=>{
          if (devices && devices.length) {  
            devices.forEach((device)=>{
              this.dispositivos.push(device)
            })
          }
        },
        ()=>{Notify.create({message:"Nenhum dispositivo já pareado foi encontrado.", type:"warning"})}
      )
    },
    async procurarDispositivos(){
      this.dispositivos = []
      Loading.show({message: 'Procurando Dispositivos'})
      bluetoothSerial.discoverUnpaired(
        (devices) => {this.dispositivos=devices},
        () => {Loading.hide();Notify.create({message:'Nenhum dispositivo encontrado.'})}
      )
      bluetoothSerial.setDeviceDiscoveredListener( (device) => {
        console.log('Device found: ', device)
        this.dispositivos.push(device)
      })
      setTimeout(() => {
        Loading.hide();
        console.log('Available devices: ', this.dispositivos)
        bluetoothSerial.clearDeviceDiscoveredListener();
        this.desconectarTodos()
      }, 20000);
    },
    async enviarDado(device,dado){
      let response
      if (this.dispConectado&&device&&this.dispConectado.id==device.id) response="sucesso"
      else{ response = await this.conectarDispositivo(device, false) }
      Loading.show({message:"Enviando comando "+dado})
      let timeout = this.dispConectado ? 0 : 5000
      this.loading = true
      if (this.dispConectado)  setTimeout(() => {
        this.loading = false
      }, this.dispConectado ? 200 : 5000);
      setTimeout(() => {
        if (response="sucesso" && this.dispConectado) {
          try{dado = new String(dado).toString()}
          catch{return}
          if (dado){
            bluetoothSerial.write( dado+"||", 
              ()=>{
                console.log('dado enviado: ', dado)
                Loading.hide()
              }, 
              ()=>{Notify.create({message:"Ocorreu um erro ao enviar a sua mensagem.", type:"negative"});Loading.hide()}
            );
          }
        }
      }, timeout);
    },
    async conectarDispositivo(device, cadastro){
      Loading.show({message:'Conectando com dispositivo '+ device.name})
      bluetoothSerial.connect(device.id, 
        (s)=>{
          console.log('Dispositivo', device.id, ' conectado.');
          let isDeviceSet = cadastro ? this.hasDevice(device) : true
          this.dispConectado = device
          if(cadastro || isDeviceSet)Loading.hide();
          if (cadastro && !isDeviceSet) {
            this.$store.commit('addDevice', device)
            this.$store.commit('setPage', 'pageOne')
          }
          bluetoothSerial.write("limparEntrada||");
          return 'sucesso'
        },
        (e)=>{console.log('Device ', device.id, ' disconnected');this.dispConectado=null;}
      )
      setTimeout(() => {
        Loading.hide()
        if (!this.dispConectado) {this.desconectarTodos();alert("Falha ao conectar com dispositivo, tente novamente.");}
      }, 10000);
    },
    desconectarTodos(){
      bluetoothSerial.disconnect(()=>{console.log("Desconectado com sucesso")}, ()=>{console.log("Erro ao desconectar")});
    },

    // ****************** FUNCÕES GERAIS *************************
    getLuzes () {
      if (this.devices){
        for (let device of this.devices) {
          const aux = { nome: device.name, estado: false, macBluetooth: device.id }
          this.luzes.push(aux)
        }
      }
    },
    getDeviceByName(info){
      let match = null;
      if(info.toLowerCase()=="um"||info.toLowerCase()=="um"||parseInt(info)==1) return 1
      else if(info.toLowerCase()=="dois"||info.toLowerCase()=="dois"||parseInt(info)==2) return 2
      else if(info.toLowerCase()=="três"||info.toLowerCase()=="três"||parseInt(info)==3) return 3
      for (let props in this.dispConectado){
        if (props=="comodos"){
          this.dispConectado[props].forEach((val, index)=>{
            if (val.name.toLowerCase() == info.toLowerCase()){
              match = index+1
            }
          })
        }
      }
      return match
    },
    hasDevice(device){
      let have = false
      if(this.devices) {
        this.devices.forEach((val, index) => {
          if (val.id == device.id) {
            alert("Dispositivo já adicionado")
            have = true
          }
        })
      }
      return have
    },
    salvarAlteracoesEquipamento(device, comodos){
      device.comodos = []
      comodos.forEach((val)=>{ if (val.name || val.name!="") device.comodos.push(val) })
      this.$store.dispatch('updateDevice', device)
    },
    excluirDipositivo(device) {
      Dialog.create({
        title:'Alerta',
        message:'Você tem certeza que deseja excluir o equipamento?',
        ok:'Sim',
        cancel:'Cancelar'
      }).onOk(()=>{
        let devices = this.devices
        let index = devices.indexOf(device)
        devices = this.drop(devices, index)
        this.$store.commit('setDevice', devices)
        this.editarEquipamentoModal = false
      })
    },
    mudarTela (tela) {
      if (tela) {
        this.$store.commit('setPage', tela)
      }
    },
    drop (array, position) {
      let aux = []
      array.forEach((val,i) => {
        if (i != position){
          aux.push(val)
        }
      })
      return aux
    },
    excluirComodo(comodo){
      let aux = []
      this.editarComodos.forEach((val)=>{
        if(comodo != val) aux.push(val)
      })
      this.editarComodos= aux
    }
  }
}
</script>

<style scoped>
.pulse-button {
  margin-bottom: 15vw;
  border: none;
  box-shadow: 0 0 0 0 #26A69A;
  border-radius: 50%;
  background-size:cover;
  cursor: pointer;
  -webkit-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  -moz-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  -ms-animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
  animation: pulse 1.25s infinite cubic-bezier(0.66, 0, 0, 1);
}

@-webkit-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@-moz-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@-ms-keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
@keyframes pulse {to {box-shadow: 0 0 0 45px rgba(232, 76, 61, 0);}}
</style>
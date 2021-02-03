<template>
  <q-page class="column content-center q-ma-sm">
    <template v-if="getPage=='inicio'">
      <div class="q-my-sm">
        <q-img
          src='C:\Users\Cliente\Desktop\Oficinas\EasyLight\src\statics\logo.png'
          alt="Logo"
        />
      </div>
      <div class="q-mt-xl q-my-sm">
        <span class="text-h4">Seja bem vindo!</span>
        <q-input class="q-my-sm" v-model="text" type="text" placeholder="Insira seu nome aqui" />
        <q-btn class="full-width" align="center" color="primary" label="ENTRAR" @click="text ? mudarTela('pageOne') : mostrarNotificacao( 'Preencha o seu nome', 'warning') " />
      </div>
    </template>

    <template v-else-if="getPage=='pageOne'">
      <q-list class="full-width q-ma-sm" bordered  v-for="(luz, index) in luzes" :key="index">
        <q-item clickable v-ripple>
          <q-item-section class="col-2" avatar>
            <q-icon color="primary" name="light" />
          </q-item-section>
          <q-item-section class="col-8">
            <span class="text-h6"> {{luz.nome}}<span style="font-size:13px;">{{luz.macBluetooth ? ' - '+luz.macBluetooth : ''}}</span> </span>
            <span class="text-h8 q-mb-sm"> Estado: {{luz.estado ? 'Ligada' : 'Desligada'}}</span>
          </q-item-section>
          <q-item-section avatar class="col-2">
            <q-icon color="secondary" name="delete" />
          </q-item-section>
        </q-item>
        <q-btn class="full-width" color="primary" :label="luz.estado ? 'Desligar' : 'Ligar'" @click="acionarLuz" />
      </q-list>
      <div class="q-mb-xl">
        <q-btn class="full-width" color="green-8" icon="add" label="Adicionar Dispositivo" />
      </div>
    </template>

    <template v-else-if="getPage=='bluetooth'">
      <div class="columns full-width flex-center">
        <div class="q-ma-lg">
          Estado do Bluetooth: <strong>Ligado</strong>
        </div>
        <q-separator class="q-mb-sm" color="black" />
        <div class="q-ma-md" v-if="dispositivos">
          <span class="text-bold q-mb-md">Dispositivos disponíveis: </span>
          <q-list bordered>
            <q-item clickable v-ripple>
              <q-item-section avatar>
                <q-icon color="primary" name="light" />
              </q-item-section>
              <q-item-section class="columns">
                <span>Nome: HC- 06</span>
                <span>Mac: 78:54:3C:D4:54</span>
              </q-item-section>
            </q-item>
          </q-list>
        </div>
        <div v-else-if="!dispositivos && bluetoothOn">
          <q-btn class="full-width q-ma-md" color="primary" icon="search" label="Procurar dispositivo" />
        </div>
      </div>
    </template>
  </q-page>
</template>

<script>
export default {
  name: 'PageIndex',
  data () {
    return {
      text: '',
      luzes: [],
      dispositivos: ['null'],
      dispConectado: '',
      bluetoothOn: true
    }
  },
  mounted () {
    this.getLuzes()
    console.log('page: ', this.getPage)
  },
  computed: {
    logo () {
      return ''
    },
    getPage () {
      // eslint-disable-next-line vue/no-side-effects-in-computed-properties
      return this.$store.getters.getPage || 'inicio'
    }
  },
  methods: {
    getLuzes () {
      const aux = { nome: 'Luz 1', estado: false, macBluetooth: '' }
      const aux2 = { nome: 'Luz 2', estado: false, macBluetooth: '' }
      const aux3 = { nome: 'Luz 3', estado: true, macBluetooth: '78:54:3C:D4:54' }
      this.luzes.push(aux)
      this.luzes.push(aux3)
      this.luzes.push(aux2)
    },
    mostrarNotificacao (mensagem, tipo) {
      if (!tipo || tipo === 'positive' || tipo === 'negative' || tipo === 'warning') {
        alert(mensagem)
      }
    },
    mudarTela (tela) {
      if (tela) {
        this.$store.commit('setPage', tela)
      }
    },
    acionarLuz () {
      console.log('Acionada')
    }
  }
}
</script>

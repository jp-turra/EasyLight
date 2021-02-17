<template>
  <q-layout view="lHh Lpr lFf">
    <q-header elevated>
      <q-toolbar>
        <q-btn
          flat
          dense
          round
          icon="menu"
          aria-label="Menu"
          @click="leftDrawerOpen = !leftDrawerOpen"
        />

        <q-toolbar-title class="flex flex-center">
          <span v-if="userName">{{userName}}</span>
          <span v-else>EasyLight</span>
        </q-toolbar-title>

        <q-icon v-if="getPage=='pageOne'" size="20px" name="logout" color="white" @click="getPage!='inicio' ? voltarInicio() : ''"/>
        <q-icon v-else-if="getPage=='bluetooth'" size="20px" name="keyboard_arrow_left" color="white" @click="$store.commit('setPage', 'pageOne')"/>
        <div v-else>v0.0.0</div>
      </q-toolbar>
    </q-header>

    <q-drawer
      v-model="leftDrawerOpen"
      show-if-above
      bordered
      content-class="bg-grey-1"
    >
      <q-list>
        <q-item-label
          header
          class="row flex-center justify-between text-grey-8"
          @click="leftDrawerOpen=false"
        >
          <strong>MENU</strong>
          <q-btn flat color="primary" icon="close" />
        </q-item-label>
        <q-separator color="black"/>
        <q-item v-show="getPage!='inicio'" clickable @click="abrirBluetooth()" >
          <q-item-section avatar >
            <q-icon name="bluetooth" color="primary"/>
          </q-item-section>

          <q-item-section>
            <q-item-label>Bluetooth</q-item-label>
          </q-item-section>
        </q-item>
        <EssentialLink
          v-for="link in essentialLinks"
          :key="link.title"
          v-bind="link"
        />
        <q-separator color="black" v-show="getPage!='inicio'"/>
        <q-item v-show="getPage!='inicio'">
          <q-item-section avatar >
            <q-icon name="logout" color="primary"/>
          </q-item-section>

          <q-item-section>
            <q-item-label>Sair</q-item-label>
          </q-item-section>
        </q-item>
      </q-list>
    </q-drawer>

    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script>
/* eslint-disable */
import EssentialLink from 'components/EssentialLink.vue'
const linksData = [
  {
    title: 'Github',
    caption: 'github.com/jp-turra/EasyLight',
    icon: 'code',
    link: 'https://github.com/jp-turra/EasyLight'
  },
  {
    title: 'Marcos Vinicio Haas Rambo',
    caption: 'Professor orientador',
    icon: 'book',
    link: 'http://www.eletrica.ufpr.br/p/professores:rambo:inicial'
  },
  {
    title: 'Sobre',
    caption: '',
    icon: 'help',
    link: ''
  }
]

export default {
  name: 'MainLayout',
  components: { EssentialLink },
  data () {
    return {
      leftDrawerOpen: false,
      essentialLinks: linksData
    }
  },
  computed: {
    userName () {
      return this.$store.getters.getUserName
    },
    getPage () {
      return this.$store.getters.getPage || 'inicio'
    }
  },
  methods: {
    abrirBluetooth () {
      this.leftDrawerOpen = false
      bluetoothClassicSerial.isEnabled(
        () => { this.$store.commit('setBluetoothState', true) },
        () => {
          if (this.$q.platform.is.android) {
            bluetoothClassicSerial.enable(
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
    voltarInicio () {
      this.$store.commit('setPage', 'inicio')
    }
  }
}
</script>

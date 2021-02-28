<template>
  <q-item
    clickable
    v-ripple
    @click="link ? open(link) : ''"
  >
    <q-item-section
      v-if="icon"
      avatar
    >
      <q-icon :name="icon" color="primary"/>
    </q-item-section>

    <q-item-section>
      <q-item-label>{{ title }}</q-item-label>
      <q-item-label caption>
        {{ caption }}
      </q-item-label>
    </q-item-section>
  </q-item>
</template>

<script>
export default {
  name: 'EssentialLink',
  props: {
    title: {
      type: String,
      required: true
    },

    caption: {
      type: String,
      default: ''
    },

    link: {
      type: String,
      default: '#'
    },

    icon: {
      type: String,
      default: ''
    }
  },
  methods: {
    open (link, target) {
      window.open(link, target)
    }
  }
}
</script>

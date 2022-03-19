<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-img :src="require('../assets/logo.png')" contain height="60"></v-img>
          <v-card-title primary-title>
            <div class="ma-auto">
              <span class="grey--text">Heating duration: {{heatingDuration}} ms</span>
              <br>
              <span class="grey--text">Trigger delay: {{triggerDelay}} ms</span>
            </div>
          </v-card-title>
          <v-btn fab large @click="fire">
            <v-icon dark>sensors</v-icon>
          </v-btn>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data () {
    return {
      nozzleCount: null,
      heatingDuration: null,
      triggerDelay: null
    }
  },
  mounted () {
    this.$ajax
      .get('/api/v1/jetpack/config')
      .then(data => {
        this.heatingDuration = data.data.heatingDuration
        this.triggerDelay = data.data.triggerDelay
      })
      .catch(error => {
        console.log(error)
      })
  },
  methods: {
    fire: function () {
      this.$ajax
        .post('/api/v1/jetpack/fire', {
          nozzle: 0
        })
        .then(data => {
          console.log('nozzle fired')
        })
        .catch(error => {
          console.log(error)
        })
    }
  }
}
</script>

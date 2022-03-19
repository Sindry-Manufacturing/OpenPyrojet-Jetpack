<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-responsive height="25px"></v-responsive>
        <v-card>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout column>
                <v-flex xs6>
                  <v-text-field
                      label="Heating Duration (µs)"
                      v-model="heatingDuration"
                      outline
                  ></v-text-field>
                </v-flex>
                <v-flex xs6>
                  <v-text-field
                      label="Trigger Delay (µs)"
                      v-model="triggerDelay"
                      outline
                  ></v-text-field>
                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn fab light large>
            <v-icon dark @click="save">save</v-icon>
          </v-btn>
        </v-card>
        <v-responsive height="25px"></v-responsive>
        <v-card>
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row>
                <v-flex xs9>
                  <v-slider v-model="nozzleId" :max="nozzleCount - 1" label="Nozzle"></v-slider>
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="nozzleId" class="mt-0" type="number"></v-text-field>
                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
          <v-btn fab light large @click="fire">
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
      heatingDuration: null,
      triggerDelay: null,
      nozzleCount: 0,
      nozzleId: 0
    }
  },
  mounted () {
    this.$ajax
      .get('/api/v1/jetpack/config')
      .then(data => {
        this.heatingDuration = data.data.heatingDuration
        this.triggerDelay = data.data.triggerDelay
        this.nozzleCount = data.data.nozzleCount
      })
      .catch(error => {
        console.log(error)
      })
  },
  methods: {
    save: function () {
      this.$ajax
        .put('/api/v1/jetpack/config', {
          heatingDuration: parseInt(this.heatingDuration),
          triggerDelay: parseInt(this.triggerDelay)
        })
        .then(data => {
          console.log('config stored')
        })
        .catch(error => {
          console.log(error)
        })
    },
    fire: function () {
      this.$ajax
        .post('/api/v1/jetpack/fire', {
          nozzle: parseInt(this.nozzleId)
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

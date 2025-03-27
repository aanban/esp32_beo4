# beo4 source table
stbl = {
    'VIDEO' :'00',
    'AUDIO' :'01',
    'VTAPE' :'05',
    'ALL'   :'0F',
    'SPDEMO':'1D',
    'LIGHT' :'1B',
}

# beo4 command table
ctbl = {
    'NUM-0'         :'00',
    'NUM-1'         :'01',
    'NUM-2'         :'02',
    'NUM-3'         :'03',
    'NUM-4'         :'04',
    'NUM-5'         :'05',
    'NUM-6'         :'06',
    'NUM-7'         :'07',
    'NUM-8'         :'08',
    'NUM-9'         :'09',
    'CLEAR'         :'0A',
    'STORE'         :'0B',
    'STANDBY'       :'0C',
    'MUTE'          :'0D',
    'INDEX'         :'0E',
    'UP'            :'1E',
    'DOWN'          :'1F',
    'TUNE'          :'20',
    'CLOCK'         :'28',
    'FORMAT'        :'2A',
    'LEFT'          :'32',
    'RETURN'        :'33',
    'RIGHT'         :'34',
    'GO'            :'35',
    'STOP'          :'36',
    'RECORD'        :'37',
    'SELECT'        :'3F',
    'SPEAKER'       :'44',
    'PICTURE'       :'45',
    'TURN'          :'46',
    'LOUDNESS'      :'48',
    'BASS'          :'4D',
    'TREBLE'        :'4E',
    'BALANCE'       :'4F',
    'LIST'          :'58',
    'MENU'          :'5C',
    'VOL-UP'        :'60',
    'VOL-DOWN'      :'64',
    'LEFT-REPEAT'   :'70',
    'RIGHT-REPEAT'  :'71',
    'UP-REPEAT'     :'72',
    'DOWN-REPEAT'   :'73',
    'GO-REPEAT'     :'75',
    'GREEN-REPEAT'  :'76',
    'YELLOW-REPEAT' :'77',
    'BLUE-REPEAT'   :'78',
    'RED-REPEAT'    :'79',
    'EXIT'          :'7F',
    'TV'            :'80',
    'RADIO'         :'81',
    'VIDEO-AUX'     :'82',
    'AUDIO-AUX'     :'83',
    'VTAPE'         :'85',
    'DVD'           :'86',
    'CAMCORD'       :'87',
    'TEXT'          :'88',
    'SP-DEMO'       :'89',
    'SAT'           :'8A',
    'PC'            :'8B',
    'DOOR-CAM'      :'8D',
    'ATAPE'         :'91',
    'CD'            :'92',
    'PHONO'         :'93',
    'ATAPE2'        :'94',
    'CD2'           :'97',
    'LIGHT'         :'9B',
    'AV'            :'BF',
    'YELLOW'        :'D4',
    'GREEN'         :'D5',
    'BLUE'          :'D8',
    'RED'           :'D9',
    'STAND'         :'f7'
}

# lookup beoSrc and beoCmd and call esphome action 
@service 
def beo4Send(beo_src, beo_cmd, beo_rpt = 1):
    source = int(stbl[beo_src],16)
    command = int(ctbl[beo_cmd],16)
    repeat = int(beo_rpt)
    esphome.esp32_beo4_tx_send_beo_code(beo_src = source , beo_cmd = command, beo_rpt = repeat)
    
# send command and compare with received
@service 
def beo4SendInfo(beo_src="AUDIO", beo_cmd="GO"):
    source = int(stbl[beo_src],16)
    command = int(ctbl[beo_cmd],16)
    beo_code = f'0x{stbl[beo_src]}{ctbl[beo_cmd]}'
    beoCode = int((source * 256) + command)
    esphome.esp32_beo4_tx_send_beo_code(beo_src = source , beo_cmd = command, beo_rpt = 1)
    task.sleep(1)
    rxBeoSrc = sensor.esp32_beo4_rx_beosource.upper()
    rxBeoCmd = sensor.esp32_beo4_rx_beocommand.upper()
    rxBeoCode= int(sensor.esp32_beo4_rx_beocode)
    res = 'OK' if rxBeoCode==beoCode else 'FAIL'
    log.info(f'{beo_code:<7} {beo_src:<5} {beo_cmd:<20} {rxBeoSrc:<5} {rxBeoCmd:<20} {res}')


# send a couple of commands
@service
def beo4Test(n_repeat = 1):
    log.info(f'beoCode sended                     received')
    log.info(f'------- -------------------------- -------------------------')
    cnt= int(n_repeat)
    for x in range (cnt):
        beo4SendInfo('AUDIO','VOL-DOWN')
        beo4SendInfo('AUDIO','MUTE')
        beo4SendInfo('AUDIO','VOL-UP')
        beo4SendInfo('LIGHT','NUM-1')
        beo4SendInfo('LIGHT','NUM-2')
        beo4SendInfo('LIGHT','NUM-3')
        beo4SendInfo('AUDIO','RADIO')
        beo4SendInfo('AUDIO','NUM-1')
        beo4SendInfo('AUDIO','NUM-2')
        beo4SendInfo('VIDEO','TV')
        beo4SendInfo('AUDIO','CD')
        beo4SendInfo('ALL'  ,'STANDBY')
        beo4SendInfo('AUDIO','GREEN')
        beo4SendInfo('AUDIO','UP')
        beo4SendInfo('AUDIO','YELLOW')
        beo4SendInfo('AUDIO','LEFT')
        beo4SendInfo('AUDIO','GO')
        beo4SendInfo('AUDIO','RIGHT')
        beo4SendInfo('AUDIO','RED')
        beo4SendInfo('AUDIO','DOWN')
        beo4SendInfo('AUDIO','BLUE')
    log.info(f'------- -------------------------- -------------------------')

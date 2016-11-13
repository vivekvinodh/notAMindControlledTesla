import sys

import numpy as np
import serial
from sklearn import decomposition
from sklearn import neighbors

import collect
import train
from util import open_bci
from util import csv_collector

if __name__ == '__main__':
  # TODO: remove hardcoded values.
  bci_port = '/dev/tty.usbserial-DQ007SU3'
  arduino_port = '/dev/tty.usbmodem1421'

  assert len(sys.argv) == 2, "Error: Please provide filename."

  filename = sys.argv[1]

  print("Writing to file: {}".format(filename))
  print("Using Port: {}".format(port))

  labels = ['go', 'stop']

  print("Starting connection with OpenBCI on port={}...".format(bci_port))
  board = open_bci.OpenBCIBoard(port=bci_port)
  collector = csv_collector.CSVCollector(filename, board)
  collector_options = CollectionOptions(3, 1, 3, 3, labels)
  print("Collecting data...")
  collect(collector, collector_options)

  data = train.read_clean(filename, labels[0], labels[1])

  Xtr, Xte, Ytr, Yte = train.prepare_train_test(data)

  ica = decomposition.FastICA(n_components=6)
  nn = neighbors.KNeighborsClassifier(n_neighbors=5)
  model = train.TransformClassifier(ica, nn)
  model.fit(Xtr, Ytr)
  print("\nModel score: {}\n".format(model.score(Xte, Yte)))

  print("Starting connection with Arduino...")
  arduino_ser = serial.Serial(arduino_port)

  def handle_sample(sample):
    y = model.predict(np.array(sample.channel_data).reshape(1, -1))[0]
    print("Prediction: {}".format(y))
    s = 1 if y == 'go' else 0
    ser_val = bytes(chr(s), 'utf8')
    print("Writing '{}' to serial...".format(ser_val))
    arduino_ser.write(ser_val)

  print("Starting stream...")
  board.start(handle_sample)

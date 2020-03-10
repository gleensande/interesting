import csv
import datetime as dt

# получение данных в нужном формате из csv
def get_data_from_csv():
    time = []
    temperature = []
    with open('NewYorkMay2018.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=';')
        line_count = 0

        for row in csv_reader:
            if line_count > 6:
                if row[1] != "" :
                    time.append(dt.datetime.strptime(row[0],'%d.%m.%Y %H:%M'))
                    temperature.append(float(row[1]))
            line_count += 1

    # порядок указания времени в архиве - обратный
    time.reverse()
    temperature.reverse()

    return time, temperature
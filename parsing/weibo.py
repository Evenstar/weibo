import pickle, json
filehandle = open('test.txt','w')
if __name__ == '__main__':
    fname = "middle_sample"
    for line in open(fname):
        if not line.strip():
            continue
        line = line.decode('utf-8')
        d = json.loads(line)
      #  if 'm' in d.keys():
       #     print d['a'],d['m'].encode('utf-8')
       # print d['e']
        #print d['a'], d['e']
        filehandle.write(d['a'])
        filehandle.write('\t')
        st= "%s"%d['e']
        filehandle.write(st.encode('utf-8'))
        filehandle.write('\t')
        filehandle.write(d['c'].encode('utf-8'))
        filehandle.write('\n')
filehandle.close()

    
 

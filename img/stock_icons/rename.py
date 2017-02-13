import os

for fname in os.listdir('.'):
    if fname == 'rename.py':
        continue
    main = fname.replace('.png', '')
    newname = main +  '.png'
    os.rename(fname, newname)

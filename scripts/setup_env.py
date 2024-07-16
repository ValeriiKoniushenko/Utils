import subprocess

result = str(subprocess.run('pip list', stdout=subprocess.PIPE).stdout.decode('utf-8'))

if not 'setuptools' in result:
    subprocess.run('pip install setuptools')
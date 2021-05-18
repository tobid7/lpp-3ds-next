echo Installing

          curl -o pacman.deb https://github.com/devkitPro/pacman/releases/download/v1.0.2/devkitpro-pacman.amd64.deb
          sudo apt-get install gdebi-core
          sudo chmod +x pacman.deb
          sudo gdebi pacman.deb
          git clone https://github.com/Tobi-D7/lpp-3ds-updated.git
          export D7=/lpp-3ds-updated
          cd $(D7)/
       
          sudo mkdir /opt/devkitpro
          cp -r 'dka/devkitARM-r45/' '/opt/devkitpro/dka-r45'
          export DEVKITPRO=/opt/devkitpro
          export DEVKITARM=/opt/devkitpro/dka-r45
          git clone https://github.com/smealum/aemstro.git aemstro
          sudo cp -r 'aemstro/' '/opt/devkitpro/aemstro'
          export AEMSTRO=/opt/devkitpro/aemstro

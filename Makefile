mod:
	geode build

mod-run:
	geode build
	steam steam://rungameid/322170

safeeyes:
	cd SafeEyes-Plugin && rm eshlistener
	cd SafeEyes-Plugin/functional-listen-server && cargo build && mv target/debug/eshlistener ../
	rm -rf ~/.config/safeeyes/plugins/esh-integration
	mkdir ~/.config/safeeyes/plugins/esh-integration
	cd SafeEyes-Plugin && cp -r ./* ~/.config/safeeyes/plugins/esh-integration

safeeyes-noclean:
	cd SafeEyes-Plugin/functional-listen-server && cargo build && mv target/debug/eshlistener ../
	rm -rf ~/.config/safeeyes/plugins/esh-integration
	mkdir ~/.config/safeeyes/plugins/esh-integration
	cd SafeEyes-Plugin && cp -r ./* ~/.config/safeeyes/plugins/esh-integration
	safeeyes --debug

all: mod safeeyes
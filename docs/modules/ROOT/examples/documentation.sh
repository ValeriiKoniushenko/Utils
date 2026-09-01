# tag::install[]
cd docs
npm install
# end::install[]

# tag::build[]
npm run docs
# end::build[]

# tag::validate[]
python3 scripts/validate_docs.py
# end::validate[]

"""empty message

Revision ID: ef96f8b5c87d
Revises: 094ebafb5908
Create Date: 2018-07-31 16:01:32.155922

"""

# revision identifiers, used by Alembic.
revision = 'ef96f8b5c87d'
down_revision = '094ebafb5908'

from alembic import op
import sqlalchemy as sa


def upgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.create_foreign_key(None, 'users', 'job_desc', ['duty'], ['job_id'])
    ### end Alembic commands ###


def downgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.drop_constraint(None, 'users', type_='foreignkey')
    ### end Alembic commands ###

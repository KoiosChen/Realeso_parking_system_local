"""empty message

Revision ID: 5208901ac936
Revises: efb0a01b3913
Create Date: 2018-07-29 16:10:34.574094

"""

# revision identifiers, used by Alembic.
revision = '5208901ac936'
down_revision = 'efb0a01b3913'

from alembic import op
import sqlalchemy as sa


def upgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.add_column('fixed_parking_space', sa.Column('status', sa.SmallInteger(), nullable=True))
    ### end Alembic commands ###


def downgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.drop_column('fixed_parking_space', 'status')
    ### end Alembic commands ###

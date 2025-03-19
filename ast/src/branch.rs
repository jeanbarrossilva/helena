// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use crate::node::{Node, UnmatchedPatternError};

#[derive(Debug)]
pub(crate) struct Branch<'a> {
  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that have been appended. A [None] being present in this [Vec] means that the lexeme is
  /// self-sufficient and, therefore, can end as-is due to the current node being a leaf one, while
  /// [Some]s hold the nodes by which the current one can be followed.
  children: Vec<Option<&'a Branch<'a>>>
}

impl<'a> Branch<'a> {
  /// Instantiates a branch.
  pub(crate) fn new(text: &'a str, column: u32, row: u32) -> Self {
    Branch {
      text,
      column,
      row,
      children: Vec::new()
    }
  }
}

impl<'a> Node for Branch<'a> {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, UnmatchedPatternError> {
    Ok(self.text)
  }
}

impl<'a> Branch<'a> {
  /// Denotes that a node with the given textual representation can be the next one.
  pub(crate) fn expect_with_text<N: Node, F: Fn(&'a Self) -> &N>(&mut self, text: &'a str, chain: F) -> &'a Self {
    self.expect(&Branch::new(text, self.next_column(text), self.next_row(text)), chain)
  }

  /// Obtains the column in which the node that follows this one is expected to be.
  pub(crate) fn next_column(&'a self, text: &'a str) -> u32 {
    if self.column() > 0 && self.next_row(text) == 0 {
      return self.column() + 1;
    }
    self.column()
  }

  /// Obtains the row in which the node that follows this one is expected to be.
  pub(crate) fn next_row(&'a self, text: &'a str) -> u32 {
    (self.row() as usize + text.len()).min(100) as u32
  }

  /// Denotes that the given node can be the next one.
  pub(crate) fn expect<N: Node, F: Fn(&'a N) -> (&'a N)>(
    &'a mut self,
    next: &'a N,
    chain: F
  ) -> &'a Self {
    self.children.push(Some(next));
    chain(next);
    &self
  }
}
